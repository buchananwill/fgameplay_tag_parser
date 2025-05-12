//
// Created by thele on 12/05/2025.
//

#ifndef GAMEPLAYTAGGENERATOR_H
#define GAMEPLAYTAGGENERATOR_H

#include <string>
#include <vector>
#include <filesystem>
#include "TagNode.h"

namespace fs = std::filesystem;

class gameplay_tag_generator {
public:
	/**
	 * Parse the text file referenced by the environment variable and produce two code files next
	 * to it: <BaseName>.generated.h and <BaseName>.generated.cpp.
	 *
	 * @param EnvVarName Name of the environment variable that stores a path to the text file.
	 * @param OutputUnit The Unit/Module name passed to #include statements in generated code.
	 *                   Example: if you pass "ParsedGameplayTag", the .cpp will contain
	 *                   `#include "ParsedGameplayTag.h"`.
	 *                   The generated file names themselves are derived from the input file.
	 *                   (optional) Defaults to the stem of the input file.
	 * @return true if successful, false otherwise.
	 */
	bool generate_from_env(const std::string &EnvVarName, const std::string &OutputUnit = "");

private:
	// ---------------------------------------------------------------------
	// Parsing helpers
	bool parse(const fs::path &FilePath);

	// ---------------------------------------------------------------------
	// Emission helpers
	bool emit_files(const fs::path &InputPath, const std::string &OutputUnit);

	void emit_node(const std::shared_ptr<TagNode> &Node, std::ofstream &Header, std::ofstream &Source, std::vector<std::string> &Segments);

	std::shared_ptr<TagNode> Root;
};



#endif //GAMEPLAYTAGGENERATOR_H
