//
// Created by thele on 12/05/2025.
//

#ifndef GAMEPLAY_TAG_VISITOR_H
#define GAMEPLAY_TAG_VISITOR_H
#include "../tag_node_tree_visitor.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class gameplay_tag_visitor : public tag_node_tree_visitor {
public:
	explicit gameplay_tag_visitor(const fs::path &input_path, const std::string &_output_unit = "")
		: tag_node_tree_visitor(input_path, _output_unit) {
	}

	bool visit_tree(const std::shared_ptr<TagNode>& root) override;

protected:
	void process_node(const TagNode &node) override;

 private:
	std::ofstream header;
	std::ofstream source;

	// Walk tree depth‑first

};



#endif //GAMEPLAY_TAG_VISITOR_H
