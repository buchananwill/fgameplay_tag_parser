//
// Created by thele on 12/05/2025.
//

#include "tag_node_tree_visitor.h"

#include <algorithm>
#include <iostream>

bool tag_node_tree_visitor::visit_tree(const std::shared_ptr<TagNode> &root) {
	segments.clear();

	if (!root) {
		std::cout << "Root was null ptr!" << std::endl;
		return false;
	}

	return visit_node(root, root);
}

bool tag_node_tree_visitor::visit_node(const std::shared_ptr<TagNode> &node, const std::shared_ptr<TagNode> &root) {
	if (!node) {
		return false;
	}

	if (node.get() != root.get()) {
		segments.push_back(node->Name);
		if (should_process_node(*node)) {
			process_node(*node);
		}
	}

	for (auto &child: node->Children) {
		if (!visit_node(child, root)) {
			std::cout << "Null ptr found in tree!" << std::endl;
		};
	}

	before_leaving_node(node, root);

	return true;
}

bool tag_node_tree_visitor::should_process_node(const TagNode &node) const {
	if (flags.empty()) {
		return true;
	}

	return std::ranges::all_of(flags, [&node](const std::string &flag) {
		return std::ranges::any_of(node.Flags, [&flag](const std::string &node_flag) {
			return node_flag == flag;
		});
	});
}

void tag_node_tree_visitor::before_leaving_node(const std::shared_ptr<TagNode> &node,
                                                const std::shared_ptr<TagNode> &root) {
	if (node.get() != root.get()) {
		segments.pop_back();
	}
}
