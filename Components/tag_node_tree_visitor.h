//
// Created by thele on 12/05/2025.
//

#ifndef TAG_NODE_TREE_VISITOR_H
#define TAG_NODE_TREE_VISITOR_H
#include <memory>
#include <filesystem>

#include "TagNode.h"


namespace fs = std::filesystem;

class tag_node_tree_visitor {

public:
	virtual ~tag_node_tree_visitor() = default;

	tag_node_tree_visitor(const fs::path &input_path, const std::string &_output_unit = "")
		: input_path(input_path),
		  output_unit(_output_unit) {
		if (output_unit.empty()) {
			output_unit = input_path.stem().string();
		}
	}

	virtual bool visit_tree(std::shared_ptr<TagNode> root);

protected:
	std::vector<std::string> segments;
	fs::path input_path;
	std::string output_unit;

	bool visit_node(const std::shared_ptr<TagNode> &node, const std::shared_ptr<TagNode> &root);
	virtual void process_node(std::shared_ptr<TagNode> node) = 0;
	virtual void before_leaving_node(std::shared_ptr<TagNode> node, std::shared_ptr<TagNode> root);


};



#endif //TAG_NODE_TREE_VISITOR_H
