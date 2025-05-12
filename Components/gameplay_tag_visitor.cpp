//
// Created by thele on 12/05/2025.
//

#include "gameplay_tag_visitor.h"

#include <fstream>
#include <iostream>

#include "helpers.h"


bool gameplay_tag_visitor::visit_tree(std::shared_ptr<TagNode> root) {
	fs::path headerPath = input_path.parent_path() / (input_path.stem().string() + ".generated.h");
	fs::path sourcePath = input_path.parent_path() / (input_path.stem().string() + ".generated.cpp");

	header = std::ofstream{headerPath};
	source = std::ofstream{sourcePath};
	if (!header || !source) {
		std::cerr << "Unable to create output files in " << input_path.parent_path() << std::endl;
		return false;
	}

	// Header file prolog
	header << "#pragma once\n";
	header << "#include \"NativeGameplayTags.h\"\n\n";

	// Source file prolog
	source << "#include \"" << output_unit << ".h\"\n\n";

	tag_node_tree_visitor::visit_tree(root);

	std::cout << "Generated: \n  " << headerPath << "\n  " << sourcePath << std::endl;
	return true;
}

void gameplay_tag_visitor::process_node(std::shared_ptr<TagNode> node) {
	std::string underscoreName = helpers::join(segments, "_");
	std::string dotName = helpers::join(segments, ".");

	header << "UE_DECLARE_GAMEPLAY_TAG_EXTERN(" << underscoreName << ")\n";

	// Escape double quotes in comments so we can safely embed them
	std::string escapedComment = node->Comment;
	helpers::replace_all(escapedComment, "\"", "\\\"");
	source << "UE_DEFINE_GAMEPLAY_TAG_COMMENT(" << underscoreName << ", \"" << dotName << "\", \"" <<
			escapedComment << "\")\n";
}


