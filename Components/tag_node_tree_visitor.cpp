//
// Created by thele on 12/05/2025.
//

#include "tag_node_tree_visitor.h"

bool tag_node_tree_visitor::visit_tree(const std::shared_ptr<TagNode> root) {
	segments.clear();

	return visit_node(root, root);
}

bool tag_node_tree_visitor::visit_node(const std::shared_ptr<TagNode> &node, const std::shared_ptr<TagNode> &root) {

	if (node.get() != root.get()) {
		segments.push_back(node->Name);
		process_node(node);
	}

	for (auto &child: node->Children) {
		visit_node(child, root);
	}

	before_leaving_node(node, root);

	return true;
}

void tag_node_tree_visitor::before_leaving_node(std::shared_ptr<TagNode> node, std::shared_ptr<TagNode> root) {
	if (node.get() != root.get()) {
		segments.pop_back();
	}
}
