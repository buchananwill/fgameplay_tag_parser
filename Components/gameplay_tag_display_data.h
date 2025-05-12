//
// Created by thele on 12/05/2025.
//

#ifndef GAMEPLAY_TAG_DISPLAY_DATA_H
#define GAMEPLAY_TAG_DISPLAY_DATA_H
#include <fstream>

#include "tag_node_tree_visitor.h"


class gameplay_tag_display_data : public tag_node_tree_visitor {
public:
	gameplay_tag_display_data(const fs::path &input_path, const std::string &_output_unit)
		: tag_node_tree_visitor(input_path, _output_unit) {
	}

	bool visit_tree(std::shared_ptr<TagNode> root) override;

protected:
	std::ofstream csv;

	void process_node(std::shared_ptr<TagNode> node) override;
};



#endif //GAMEPLAY_TAG_DISPLAY_DATA_H
