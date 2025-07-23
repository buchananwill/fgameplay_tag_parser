//
// Created by thele on 23/07/2025.
//

#ifndef GAMEPLAY_TAG_TO_FRAGMENT_VISITOR_H
#define GAMEPLAY_TAG_TO_FRAGMENT_VISITOR_H
#include <fstream>

#include "../tag_node_tree_visitor.h"

struct fragment_file_paths {
	std::filesystem::path fragments;
	std::filesystem::path header;
	std::filesystem::path cpp;
};

struct fragment_files {
	std::ofstream fragments;
	std::ofstream header;
	std::ofstream cpp;
};

struct fragment_templates {
};

class gameplay_tag_to_fragment_visitor : public tag_node_tree_visitor {
public:
	gameplay_tag_to_fragment_visitor(
		const fs::path& input_path
		, const std::string& _output_unit
		, const std::string& processor_name
		, const std::string& fragments_header_suffix = ""
		, const std::string& fragment_dispatch_header = ""
		, const std::string& tag_list_header = ""
	)
		: tag_node_tree_visitor(input_path, _output_unit),
		  processor_name(processor_name)
		  , tag_to_fragment_dispatch_header_name(fragment_dispatch_header)
		  , tag_list_header_name(tag_list_header)
		  , fragments_header_suffix(fragments_header_suffix) {
		generated_dir = input_path.parent_path() / "Generated";
	}

	bool visit_tree(const std::shared_ptr<TagNode>& root) override;

protected:
	void process_node(const TagNode& node) override;

	fs::path generated_dir;
	std::string includes_buffer;

	std::string processor_name;
	std::string fragments_header_suffix;
	std::string underscore_list_buffer;
	std::string tag_to_fragment_dispatch_header_name;
	std::string tag_list_header_name;

	virtual void buffer_node_strings(const TagNode& node) = 0;

	virtual void write_fragments_file(const TagNode& node, std::ofstream& stream) const = 0;

	virtual void write_processor_header_file(const TagNode& node, std::ofstream& stream) const = 0;

	virtual void write_processor_cpp_file(const TagNode& node, std::ofstream& stream) const = 0;

	virtual std::string format_canonical_list(const std::string& underscore_list_buffer) const = 0;

	virtual std::string format_tag_to_dispatch() const = 0;

	bool conditionally_write_canonical_list() const;

	bool conditionally_write_tag_to_dispatch() const;

private:
	fragment_file_paths make_file_paths(const TagNode& node);

};


#endif //GAMEPLAY_TAG_TO_FRAGMENT_VISITOR_H
