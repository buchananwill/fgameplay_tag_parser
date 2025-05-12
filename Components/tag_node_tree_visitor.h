//
// Created by thele on 12/05/2025.
//

#ifndef TAG_NODE_TREE_VISITOR_H
#define TAG_NODE_TREE_VISITOR_H
#include <memory>

#include "TagNode.h"


class tag_node_tree_visitor {

public:
	virtual ~tag_node_tree_visitor() = default;

	virtual bool visit_tree(std::shared_ptr<TagNode> root);

protected:
	bool visit_node(const std::shared_ptr<TagNode> &node, const std::shared_ptr<TagNode> &root);
	virtual void process_node(std::shared_ptr<TagNode> node) = 0;
	virtual void before_leaving_node(std::shared_ptr<TagNode> node, std::shared_ptr<TagNode> root);

	std::vector<std::string> segments;

};



#endif //TAG_NODE_TREE_VISITOR_H
