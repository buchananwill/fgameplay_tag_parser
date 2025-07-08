//
// Created by thele on 12/05/2025.
//

#include "gameplay_tag_display_data.h"

#include <format>
#include <iostream>

#include "../helpers.h"

bool gameplay_tag_display_data::visit_tree(const std::shared_ptr<TagNode>& root) {
	const fs::path csvPath = input_path.parent_path() / (input_path.stem().string() + ".csv");


	csv = std::ofstream{csvPath};

	if (!csv) {
		std::cerr << "Unable to create output files in " << input_path.parent_path() << std::endl;
		return false;
	}

	// Header file prolog
	csv << "RowName,DisplayName,GameplayTag\n";

	tag_node_tree_visitor::visit_tree(root);

	std::cout << "Generated: \n  " << csvPath << std::endl;
	return true;
}

void gameplay_tag_display_data::process_node(const TagNode &node) {
	std::string row_name = helpers::join(segments, ".");

	int num_namespace_segments = std::max(std::min(static_cast<int>(segments.size()-1), 3), 1);
	std::vector<std::string> namespace_prefix_segments;
	for (int i = 0; i < num_namespace_segments && i < segments.size(); ++i) {
		namespace_prefix_segments.emplace_back(segments[i]);
	}
	std::string namespace_prefix = helpers::join(namespace_prefix_segments, "_");

	int num_literal_segments = std::min(static_cast<int>(segments.size()), 3);
	std::vector<std::string> string_literal_segments;
	for (int i = static_cast<int>(segments.size()) - num_literal_segments; i < segments.size(); ++i) {
		if (i >= 0) {
			string_literal_segments.emplace_back(segments[i]);
		}
	}
	std::string string_literal = helpers::join(string_literal_segments, " ");

	auto display_name = std::format(R"csv_loc(,"NSLOCTEXT(""{}"", ""{}"", ""{}"")")csv_loc", namespace_prefix, row_name,
	                                string_literal);
	auto tag_name = std::format(R"tag_name(,"(TagName=""{}"")")tag_name", row_name);

	csv << row_name
			<< display_name
			<< tag_name
			<< "\n";
}
