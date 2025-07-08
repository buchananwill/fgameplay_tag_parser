//
// Created by thele on 07/07/2025.
//

#ifndef GAMEPLAY_TAG_SUPPLIER_PROCESSOR_H
#define GAMEPLAY_TAG_SUPPLIER_PROCESSOR_H

#include "../tag_node_tree_visitor.h"

class gameplay_tag_supplier_processor : public tag_node_tree_visitor {
public:
	gameplay_tag_supplier_processor(const fs::path &input_path, const std::string &_output_unit)
		: tag_node_tree_visitor(input_path, _output_unit) {
	}

	bool visit_tree(const std::shared_ptr<TagNode>& root) override;

protected:
	void process_node(const TagNode &node) override;
};

#endif //GAMEPLAY_TAG_SUPPLIER_PROCESSOR_H
