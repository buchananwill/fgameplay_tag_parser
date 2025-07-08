//
// Created by thele on 12/05/2025.
//

#ifndef TAG_NODE_TREE_VISITOR_H
#define TAG_NODE_TREE_VISITOR_H
#include <memory>
#include <filesystem>
#include <utility>

#include "TagNode.h"


namespace fs = std::filesystem;

class tag_node_tree_visitor {

public:
	virtual ~tag_node_tree_visitor() = default;

	explicit tag_node_tree_visitor(const fs::path &input_path, std::string  _output_unit = "")
		: input_path(input_path),
		  output_unit(std::move(_output_unit)) {
		if (output_unit.empty()) {
			output_unit = input_path.stem().string();
		}
	}

	virtual bool visit_tree(const std::shared_ptr<TagNode>& root);

protected:
	std::vector<std::string> segments;
	fs::path input_path;
	std::string output_unit;

	bool visit_node(const std::shared_ptr<TagNode> &node, const std::shared_ptr<TagNode> &root);
	virtual void process_node(const TagNode &node) = 0;
	virtual void before_leaving_node(const std::shared_ptr<TagNode>& node, const std::shared_ptr<TagNode>& root);


};



#endif //TAG_NODE_TREE_VISITOR_H
