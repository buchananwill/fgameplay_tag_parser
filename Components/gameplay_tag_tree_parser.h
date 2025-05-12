//
// Created by thele on 12/05/2025.
//

#ifndef GAMEPLAYTAGGENERATOR_H
#define GAMEPLAYTAGGENERATOR_H

#include <string>

#include <filesystem>
#include "TagNode.h"
#include "tag_node_tree_visitor.h"

namespace fs = std::filesystem;

class gameplay_tag_tree_parser {
public:

	bool parse_from_file_path(const fs::path &FilePath);

	std::vector<std::unique_ptr<tag_node_tree_visitor>> tree_visitors;

private:

	bool parse(const fs::path &FilePath);

	std::shared_ptr<TagNode> root;
};



#endif //GAMEPLAYTAGGENERATOR_H
