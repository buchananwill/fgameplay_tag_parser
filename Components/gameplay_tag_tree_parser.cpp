//
// Created by thele on 12/05/2025.
//

#include "gameplay_tag_tree_parser.h"
#include <fstream>
#include <iostream>
#include <stack>

#include "helpers.h"


using shared_tag_node = std::shared_ptr<TagNode>;
using unique_tag_node = std::unique_ptr<TagNode>;
auto constexpr not_present = std::string::npos;

namespace {
	std::string split_comment(TagNode &node, const std::string &input_line) {
		if (const auto hashPos = input_line.find('#'); hashPos != not_present) {
			node.Comment = input_line.substr(hashPos + 1); // no '#'
			helpers::trim(node.Comment);

			return input_line.substr(0, hashPos);
		}

		return input_line;
	}

	void separate_flags(TagNode &node, std::string &token_and_optional_flags) {
		auto next_flag = token_and_optional_flags.rfind('$');
		while (next_flag != not_present) {
			if (next_flag + 1 < token_and_optional_flags.length()) {
				auto flag_string = token_and_optional_flags.substr(next_flag + 1);
				helpers::trim(flag_string);
				helpers::remove_spaces(flag_string);
				if (!flag_string.empty()) {
					node.Flags.push_back(flag_string);
				}
			}

			token_and_optional_flags = token_and_optional_flags.substr(0, next_flag);
			next_flag = token_and_optional_flags.rfind('$');
		}
	}

	TagNode make_tag_node(const std::string &input_line) {
		TagNode response;

		// Split token and inline comment ("name [...$flag] # comment")
		std::string token_and_optional_flags = split_comment(response, input_line);

		helpers::trim(token_and_optional_flags);

		separate_flags(response, token_and_optional_flags);

		response.Name = token_and_optional_flags;

		helpers::remove_spaces(response.Name);
		helpers::trim(response.Name);

		return response;
	}

	bool line_is_not_a_node(const std::string &stripped) {
		return stripped.empty() || stripped[0] == '#';
	}

}


bool gameplay_tag_tree_parser::parse_from_file_path(const fs::path &FilePath) {
	if (!parse(FilePath)) {
		return false;
	};

	for (auto &tree_visitor: tree_visitors) {
		if (tree_visitor == nullptr) {
			continue;
		}
		tree_visitor->visit_tree(root);
	}

	return true;
}



bool gameplay_tag_tree_parser::parse(const fs::path &FilePath) {
	std::ifstream in(FilePath);
	if (!in) {
		std::cerr << "Failed to open " << FilePath << std::endl;
		return false;
	}

	// Empty root
	root = std::make_shared<TagNode>();
	root->Name = "";

	std::stack<std::pair<int, shared_tag_node> > indentStack;
	indentStack.emplace(-1, root.get());

	std::string line;
	size_t lineNumber = 0;
	while (std::getline(in, line)) {
		++lineNumber;

		helpers::clean(line, lineNumber);

		int indent = helpers::count_indent(line);

		helpers::trim(line);

		if (line_is_not_a_node(line))
			continue;

		auto node = std::make_shared<TagNode>(make_tag_node(line));

		if (node->Name.empty()) {
			continue;
		}

		// Move up the stack until we find parent with smaller indent
		while (!indentStack.empty() && indentStack.top().first >= indent) {
			indentStack.pop();
		}
		if (indentStack.empty()) {
			std::cerr << "Malformed indentation at line " << lineNumber << std::endl;
			return false;
		}
		auto parent = indentStack.top().second;
		node->Parent = parent;
		parent->Children.push_back(node);
		indentStack.emplace(indent, node);
	}

	return true;
}
