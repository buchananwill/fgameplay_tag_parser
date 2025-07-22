//
// Created by thele on 22/07/2025.
//

#ifndef GAMEPLAY_TAG_SCALAR_FIELD_ACCUMULATOR_H
#define GAMEPLAY_TAG_SCALAR_FIELD_ACCUMULATOR_H
#include "../tag_node_tree_visitor.h"


class gameplay_tag_scalar_field_accumulator : public tag_node_tree_visitor {
public:
	gameplay_tag_scalar_field_accumulator(const fs::path &input_path, const std::string &_output_unit)
		: tag_node_tree_visitor(input_path, _output_unit) {
	}

	bool visit_tree(const std::shared_ptr<TagNode>& root) override;

protected:
	void process_node(const TagNode &node) override;

	fs::path header_dir;
	fs::path cpp_dir;
	std::string includes_buffer;
	std::string if_accumulator_branches_buffer;
	std::string if_field_emitters_branches_buffer;
	std::string underscore_list_buffer;
	std::string csv_buffer;

	bool conditionally_write_canonical_list() const;
	bool conditionally_write_tag_to_dispatch() const;
};



#endif //GAMEPLAY_TAG_SCALAR_FIELD_ACCUMULATOR_H
