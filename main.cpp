// GameplayTagGenerator.cpp
// A utility class to parse an indentation‑based text file describing gameplay tags
// and emit UE_DECLARE_GAMEPLAY_TAG_EXTERN / UE_DEFINE_GAMEPLAY_TAG_COMMENT macros
// into companion .h and .cpp files.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

struct TagNode {
	std::string Name; // Tag name token
	std::string Comment; // Raw comment text (optional)
	std::vector<TagNode *> Children; // Child nodes
	TagNode *Parent = nullptr; // Parent pointer for convenience
};

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
	bool GenerateFromEnv(const std::string &EnvVarName, const std::string &OutputUnit = "") {
		const char *pathCStr = std::getenv(EnvVarName.c_str());
		if (!pathCStr) {
			std::cerr << "Environment variable " << EnvVarName << " is not defined." << std::endl;
			return false;
		}
		fs::path inputPath(pathCStr);
		if (!fs::exists(inputPath)) {
			std::cerr << "Input file " << inputPath << " does not exist." << std::endl;
			return false;
		}
		if (!Parse(inputPath)) {
			return false;
		}
		return EmitFiles(inputPath, OutputUnit.empty() ? inputPath.stem().string() : OutputUnit);
	}

private:
	// ---------------------------------------------------------------------
	// Parsing helpers
	bool Parse(const fs::path &FilePath) {
		std::ifstream in(FilePath);
		if (!in) {
			std::cerr << "Failed to open " << FilePath << std::endl;
			return false;
		}

		Root = std::make_unique<TagNode>();
		Root->Name = ""; // Dummy root

		std::stack<std::pair<int, TagNode *> > indentStack; // pair<indentLevel, node>
		indentStack.push({-1, Root.get()});

		std::string line;
		size_t lineNumber = 0;
		while (std::getline(in, line)) {
			++lineNumber;
			// Strip trailing CR for Windows files
			if (!line.empty() && line.back() == '\r')
				line.pop_back();

			// Handle UTF‑8 BOM on the very first line
			if (lineNumber == 1 && line.rfind("\xEF\xBB\xBF", 0) == 0)
				line.erase(0, 3);

			// Skip completely empty lines
			if (line.find_first_not_of(" \t") == std::string::npos)
				continue;

			int indent = CountIndent(line);
			std::string stripped = StripIndent(line);

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
				Trim(comment);
			} else {
				token = stripped;
			}

			Trim(token);

			if (token.empty()) {
				continue; // line was only a comment after indent
			}

			// Remove spaces within tokens
			std::vector<size_t> spaces;
			for (int i = 0; i < token.size(); ++i) {
				if (token[i] == ' ') {
					spaces.push_back(i);
				}
			}

			for (int i = static_cast<int>(spaces.size() - 1); i >= 0 ; --i) {
				token.erase(spaces[i], 1);
			}

			// Move up the stack until we find parent with smaller indent
			while (!indentStack.empty() && indentStack.top().first >= indent) {
				indentStack.pop();
			}
			if (indentStack.empty()) {
				std::cerr << "Malformed indentation at line " << lineNumber << std::endl;
				return false;
			}
			TagNode *parent = indentStack.top().second;
			auto *node = new TagNode{token, comment, {}, parent};
			parent->Children.push_back(node);
			indentStack.emplace(indent, node);
		}

		return true;
	}

	static int CountIndent(const std::string &Line) {
		int count = 0;
		for (char c: Line) {
			if (c == ' ')
				++count;
			else if (c == '\t')
				count += 4; // Treat tab as 4 spaces
			else
				break;
		}
		return count;
	}

	static std::string StripIndent(const std::string &Line) {
		size_t first = Line.find_first_not_of(" \t");
		return Line.substr(first == std::string::npos ? Line.size() : first);
	}

	static void Trim(std::string &s) {
		size_t first = s.find_first_not_of(" \t");
		size_t last = s.find_last_not_of(" \t");
		if (first == std::string::npos)
			s.clear();
		else
			s = s.substr(first, last - first + 1);
	}

	// ---------------------------------------------------------------------
	// Emission helpers
	bool EmitFiles(const fs::path &InputPath, const std::string &OutputUnit) {
		fs::path headerPath = InputPath.parent_path() / (InputPath.stem().string() + ".generated.h");
		fs::path sourcePath = InputPath.parent_path() / (InputPath.stem().string() + ".generated.cpp");

		std::ofstream header(headerPath);
		std::ofstream source(sourcePath);
		if (!header || !source) {
			std::cerr << "Unable to create output files in " << InputPath.parent_path() << std::endl;
			return false;
		}

		// Header file prolog
		header << "#pragma once\n";
		header << "#include \"NativeGameplayTags.h\"\n\n";

		// Source file prolog
		source << "#include \"" << OutputUnit << ".h\"\n\n";

		// Walk tree depth‑first
		std::vector<std::string> segments;
		EmitNode(Root.get(), header, source, segments);

		std::cout << "Generated: \n  " << headerPath << "\n  " << sourcePath << std::endl;
		return true;
	}

	void EmitNode(TagNode *Node, std::ofstream &Header, std::ofstream &Source, std::vector<std::string> &Segments) {
		if (Node != Root.get()) // Skip dummy root
		{
			Segments.push_back(Node->Name);
			std::string underscoreName = Join(Segments, "_");
			std::string dotName = Join(Segments, ".");

			Header << "UE_DECLARE_GAMEPLAY_TAG_EXTERN(" << underscoreName << ")\n";

			// Escape double quotes in comments so we can safely embed them
			std::string escapedComment = Node->Comment;
			ReplaceAll(escapedComment, "\"", "\\\"");
			Source << "UE_DEFINE_GAMEPLAY_TAG_COMMENT(" << underscoreName << ", \"" << dotName << "\", \"" <<
					escapedComment << "\")\n";
		}

		for (auto *child: Node->Children) {
			EmitNode(child, Header, Source, Segments);
		}

		if (Node != Root.get()) {
			Segments.pop_back();
		}
	}

	static std::string Join(const std::vector<std::string> &Parts, const char *Delim) {
		std::ostringstream oss;
		for (size_t i = 0; i < Parts.size(); ++i) {
			if (i != 0) oss << Delim;
			oss << Parts[i];
		}
		return oss.str();
	}

	static void ReplaceAll(std::string &str, const std::string &from, const std::string &to) {
		if (from.empty()) return;
		size_t start = 0;
		while ((start = str.find(from, start)) != std::string::npos) {
			str.replace(start, from.length(), to);
			start += to.length();
		}
	}

private:
	std::unique_ptr<TagNode> Root;
};

// Optional: simple command‑line entry point when building as a standalone tool
int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Usage: GameplayTagGenerator <EnvVarName> [OutputUnit]" << std::endl;
		return 1;
	}
	GameplayTagGenerator gen;
	if (!gen.GenerateFromEnv(argv[1], argc > 2 ? argv[2] : "")) {
		return 2;
	}
	return 0;
}
