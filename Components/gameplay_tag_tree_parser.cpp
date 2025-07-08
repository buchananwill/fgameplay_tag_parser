//
// Created by thele on 12/05/2025.
//

#include "gameplay_tag_tree_parser.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <cstdlib>

#include "helpers.h"


bool gameplay_tag_tree_parser::parse_from_file_path(const fs::path &FilePath) {

	if (!parse(FilePath)) {
		return false;
	};

	for (auto& tree_visitor: tree_visitors) {
		if (tree_visitor == nullptr) {
			continue;
		}
		tree_visitor->visit_tree(root);
	}

	return true;
}



bool gameplay_tag_tree_parser::parse(const fs::path &FilePath) {
	using tag_node = std::shared_ptr<TagNode>;
	std::ifstream in(FilePath);
	if (!in) {
		std::cerr << "Failed to open " << FilePath << std::endl;
		return false;
	}

	// Empty root
	root = std::make_shared<TagNode>();
	root->Name = "";

	std::stack<std::pair<int, tag_node>> indentStack;
	indentStack.emplace(-1, root.get());

	std::string line;
	size_t lineNumber = 0;
	while (std::getline(in, line)) {
		++lineNumber;

		helpers::clean(line, lineNumber);

		int indent = helpers::count_indent(line);

		auto stripped = helpers::strip_indent(line);

		// Ignore whole‑line comments beginning with '#'
		if (!stripped.empty() && stripped[0] == '#')
			continue;


		// Split token and inline comment ("name   # comment")
		std::string token;
		std::string comment;
		size_t hashPos = stripped.find('#');
		if (hashPos != std::string::npos) {
			token = stripped.substr(0, hashPos);
			comment = stripped.substr(hashPos + 1); // no '#'

			// Trim whitespace around pieces
			helpers::trim(comment);
		} else {
			token = stripped;
		}

		helpers::trim(token);

		if (token.empty()) {
			continue; // line was only a comment after indent
		}

		helpers::remove_spaces(token);

		// Move up the stack until we find parent with smaller indent
		while (!indentStack.empty() && indentStack.top().first >= indent) {
			indentStack.pop();
		}
		if (indentStack.empty()) {
			std::cerr << "Malformed indentation at line " << lineNumber << std::endl;
			return false;
		}
		auto parent = indentStack.top().second;
		auto node = std::make_shared<TagNode>(TagNode{token, comment, {}, parent});
		parent->Children.push_back(node);
		indentStack.emplace(indent, node);
	}

	return true;
}


