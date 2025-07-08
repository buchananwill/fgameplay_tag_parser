// GameplayTagGenerator.cpp
// A utility class to parse an indentation‑based text file describing gameplay tags
// and emit UE_DECLARE_GAMEPLAY_TAG_EXTERN / UE_DEFINE_GAMEPLAY_TAG_COMMENT macros
// into companion .h and .cpp files.
//

#include <fstream>
#include <iostream>

#include "Components/visitors/gameplay_tag_display_data.h"
#include "Components/gameplay_tag_tree_parser.h"
#include "Components/visitors/gameplay_tag_visitor.h"

// Optional: simple command‑line entry point when building as a standalone tool
int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Usage: GameplayTagGenerator <EnvVarName> [OutputUnit]" << std::endl;
		return 1;
	}
	gameplay_tag_tree_parser gen;

	auto input_path = argv[1];

	const char *pathCStr = std::getenv(input_path);
	if (!pathCStr) {
		std::cerr << "Environment variable " << input_path << " is not defined." << std::endl;
		return 2;
	}
	fs::path inputPath(pathCStr);
	if (!fs::exists(inputPath)) {
		std::cerr << "Input file " << inputPath << " does not exist." << std::endl;
		return 2;
	}

	std::string output_path = argc > 2 ? argv[2] : "";

	gen.tree_visitors.emplace_back(std::make_unique<gameplay_tag_visitor>(gameplay_tag_visitor{inputPath, output_path}));
	gen.tree_visitors.emplace_back(std::make_unique<gameplay_tag_display_data>(gameplay_tag_display_data{inputPath, output_path}));

	if (!gen.parse_from_file_path(inputPath)) {
		return 2;
	}
	return 0;
}
