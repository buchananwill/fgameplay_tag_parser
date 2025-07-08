//
// Created by thele on 12/05/2025.
//

#include "tag_node_tree_visitor.h"

#include <iostream>

bool tag_node_tree_visitor::visit_tree(const std::shared_ptr<TagNode>& root) {
	segments.clear();

	if (!root) {
		std::cout << "Root was null ptr!" << std::endl;
		return false;
	}

	return visit_node(root, root);
}

bool tag_node_tree_visitor::visit_node(const std::shared_ptr<TagNode>& node, const std::shared_ptr<TagNode>& root) {
	if (!node) {
		return false;
	}

	if (node.get() != root.get()) {
		segments.push_back(node->Name);
		process_node(*node);
	}

	for (auto &child: node->Children) {
		if (!visit_node(child, root)) {
			std::cout << "Null ptr found in tree!" << std::endl;
		};
	}

	before_leaving_node(node, root);

	return true;
}

void tag_node_tree_visitor::before_leaving_node(const std::shared_ptr<TagNode>& node, const std::shared_ptr<TagNode>& root) {
	if (node.get() != root.get()) {
		segments.pop_back();
	}
}
