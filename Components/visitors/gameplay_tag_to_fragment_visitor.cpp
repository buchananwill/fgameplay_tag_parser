//
// Created by thele on 23/07/2025.
//

#include "gameplay_tag_to_fragment_visitor.h"

#include <iostream>

namespace {
	bool conditionally_write_output_file(
		const fs::path& dir,
		const std::string& file_name,
		auto with_file
	) {
		auto output_file_path = dir / file_name;

		std::ofstream output_filestream(output_file_path);

		if (!output_filestream) {
			std::cout << "Couldn't open tag to dispatch file\n";
			return false;
		}

		with_file(output_filestream);

		output_filestream.close();
		return true;
	}
}

bool gameplay_tag_to_fragment_visitor::visit_tree(const std::shared_ptr<TagNode>& root) {
	fs::create_directories(generated_dir);

	if (!tag_node_tree_visitor::visit_tree(root)) {
		return false;
	};

	bool success = conditionally_write_tag_to_dispatch();
	success = conditionally_write_canonical_list() ? success : false;

	return success;
}

void gameplay_tag_to_fragment_visitor::process_node(const TagNode& node) {
	auto [fragments, header, cpp] = make_file_paths(node);

	buffer_node_strings(node);

	// TODO: better way to opt out of additional fragment file creation.
	if (!fragments_header_suffix.empty()) {
		auto fragments_file = std::ofstream{fragments};

		if (fragments_file) {
			write_fragments_file(node, fragments_file);
		}

		fragments_file.close();
	}

	std::ofstream header_file{header};
	std::ofstream cpp_file{cpp};

	if (header_file) {
		write_processor_header_file(node, header_file);
		header_file.close();
	}

	if (cpp_file) {
		write_processor_cpp_file(node, cpp_file);
		cpp_file.close();
	}
}

bool gameplay_tag_to_fragment_visitor::conditionally_write_canonical_list() const {
	if (underscore_list_buffer.empty() || tag_list_header_name.empty()) {
		return false;
	}

	return conditionally_write_output_file(
		generated_dir, tag_list_header_name,
		[this](std::ofstream& c_l_file) {
			c_l_file << format_canonical_list(underscore_list_buffer);
		});
}

bool gameplay_tag_to_fragment_visitor::conditionally_write_tag_to_dispatch() const {
	if (includes_buffer.empty() || tag_to_fragment_dispatch_header_name.empty()) {
		return false;
	}

	return conditionally_write_output_file(
		generated_dir,
		tag_to_fragment_dispatch_header_name,
		[this](std::ofstream& dispatch_file) {
			dispatch_file << format_tag_to_dispatch();
		}
	);
}

fragment_file_paths gameplay_tag_to_fragment_visitor::make_file_paths(const TagNode& node) {
	return {
		generated_dir / (node.Name + fragments_header_suffix),
		generated_dir / (node.Name + processor_name + "Processor.h"),
		generated_dir / (node.Name + processor_name + "Processor.cpp")
	};
}

