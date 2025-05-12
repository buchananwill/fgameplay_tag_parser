//
// Created by thele on 12/05/2025.
//

#ifndef GAMEPLAY_TAG_VISITOR_H
#define GAMEPLAY_TAG_VISITOR_H
#include "tag_node_tree_visitor.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class gameplay_tag_visitor : public tag_node_tree_visitor {
public:
	gameplay_tag_visitor(const fs::path &input_path, const std::string &_output_unit = "")
		: input_path(input_path),
		  output_unit(_output_unit) {
		if (output_unit.empty()) {
			output_unit = input_path.stem().string();
		}
	}

	bool visit_tree(std::shared_ptr<TagNode> root) override;

protected:
	void process_node(std::shared_ptr<TagNode> node) override;

 private:
	fs::path input_path;
	std::string output_unit;
	std::ofstream header;
	std::ofstream source;

	// Walk tree depth‑first

};



#endif //GAMEPLAY_TAG_VISITOR_H
