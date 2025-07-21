//
// Created by thele on 07/07/2025.
//

#include "gameplay_tag_supplier_processor.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>

#include "flags.h"
#include "SupplierAttributeFormatTemplates.h"
#include "../helpers.h"

namespace fs = std::filesystem;

using namespace Templates::Product;

bool gameplay_tag_supplier_processor::conditionally_write_canonical_list() const {
	if (underscore_list_buffer.empty()) {
		return false;
	}

	auto canonical_list_file_path = header_dir / "SupplierAttributeTagList.h";

	std::ofstream c_l_file(canonical_list_file_path);

	if (!c_l_file) {
		std::cout << "Couldn't open tag to dispatch file\n";
		return true;
	}

	c_l_file << std::format(canonical_supplier_attributes, underscore_list_buffer);
	c_l_file.close();
	return false;
}

bool gameplay_tag_supplier_processor::conditionally_write_tag_to_dispatch() const {
	if (includes_buffer.empty() || if_attribute_branches_buffer.empty()) {
		return false;
	}

	auto tag_to_dispatch_file_path = header_dir / "TagToFragmentDispatch.h";

	std::ofstream t_to_d_file(tag_to_dispatch_file_path);

	if (!t_to_d_file) {
		std::cout << "Couldn't open tag to dispatch file\n";
		return true;
	}

	t_to_d_file << std::format(tag_to_fragment_dispatch, includes_buffer, if_attribute_branches_buffer,
	                           if_archetypes_branches_buffer);
	t_to_d_file.close();
	return false;
}

bool gameplay_tag_supplier_processor::write_csv_template() const {

	auto csv_weighting_template = header_dir / "utility_weighting_template.csv";

	std::ofstream weighting_csv(csv_weighting_template);

	if (!weighting_csv) {
		std::cout << "Couldn't open tag to dispatch file\n";
		return true;
	}

	weighting_csv << csv_buffer;
	weighting_csv.close();
	return false;
}

bool gameplay_tag_supplier_processor::visit_tree(const std::shared_ptr<TagNode> &root) {
	header_dir = input_path.parent_path() / "Public";
	cpp_dir = input_path.parent_path() / "Private";

	fs::create_directories(header_dir);
	fs::create_directories(cpp_dir);

	csv_buffer.append(csv_header);

	if (!tag_node_tree_visitor::visit_tree(root)) {
		return false;
	};


	conditionally_write_tag_to_dispatch();

	conditionally_write_canonical_list();

	write_csv_template();

	return true;
}

void gameplay_tag_supplier_processor::process_node(const TagNode &node) {
	if (!std::ranges::any_of(node.Flags, [](const std::string &flag) {
		return flag == visitor::flags::supplier_attribute;
	})) {
		return;
	}

	auto fragments_file_path = header_dir / (node.Name + "Fragments.h");
	auto header_file_path = header_dir / (node.Name + "Processor.h");
	auto cpp_file_path = cpp_dir / (node.Name + "Processor.cpp");

	std::ofstream fragments_file(fragments_file_path);
	std::ofstream header_file(header_file_path);
	std::ofstream cpp_file(cpp_file_path);

	if (!header_file || !cpp_file || !fragments_file) {
		std::cout << "Couldn't open file(s)\n";
		return;
	}

	// Buffer strings for later
	{
		includes_buffer.append(std::format(include_wrapper, node.Name));
		includes_buffer.append("\n");

		const auto underscore_name = helpers::join(segments, "_");
		const auto dot_name = helpers::join(segments, ".");

		if_attribute_branches_buffer.append(std::format(if_attribute_template, underscore_name, node.Name));
		if_archetypes_branches_buffer.append(std::format(if_archetypes_template, underscore_name, node.Name));

		if (!underscore_list_buffer.empty()) {
			underscore_list_buffer.append(",\n            ");
		}
		underscore_list_buffer.append(underscore_name);

		csv_buffer.append("\n")
				.append(std::format(weighting_csv_row, dot_name));
	}

	fragments_file << std::format(fragments_template, node.Name);
	fragments_file.close();

	header_file << std::format(header_template, node.Name);
	header_file.close();

	cpp_file << std::format(cpp_template, node.Name);
	cpp_file.close();
}
