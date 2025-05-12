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

class GameplayTagGenerator {
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
	bool GenerateFromEnv(const std::string &EnvVarName, const std::string &OutputUnit = "");

private:
	// ---------------------------------------------------------------------
	// Parsing helpers
	bool Parse(const fs::path &FilePath);

	static int CountIndent(const std::string &Line);

	static void Trim(std::string &s);

	// ---------------------------------------------------------------------
	// Emission helpers
	bool EmitFiles(const fs::path &InputPath, const std::string &OutputUnit);

	void EmitNode(std::shared_ptr<TagNode> Node, std::ofstream &Header, std::ofstream &Source, std::vector<std::string> &Segments);

	static std::string Join(const std::vector<std::string> &Parts, const char *Delim);

	static void ReplaceAll(std::string &str, const std::string &from, const std::string &to);

	std::shared_ptr<TagNode> Root;
};



#endif //GAMEPLAYTAGGENERATOR_H
