//
// Created by thele on 12/05/2025.
//

#include "gameplay_tag_generator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <cstdlib>

namespace {
	void strip_carriage_return(std::string& line) {
		if (!line.empty() && line.back() == '\r')
			line.pop_back();
	}

	void strip_bom(std::string& line, size_t lineNumber) {
		if (lineNumber == 1 && line.rfind("\xEF\xBB\xBF", 0) == 0)
			line.erase(0, 3);
	}

	std::string strip_indent(const std::string& line) {
		const size_t first = line.find_first_not_of(" \t");
		return line.substr(first == std::string::npos ? line.size() : first);
	}

	void clean(std::string& line, size_t lineNumber) {
		strip_carriage_return(line);
		strip_bom(line, lineNumber);
	}

	void remove_spaces(std::string& token) {
		std::vector<size_t> spaces;
		for (int i = 0; i < token.size(); ++i) {
			if (token[i] == ' ') {
				spaces.push_back(i);
			}
		}

		for (int i = static_cast<int>(spaces.size() - 1); i >= 0; --i) {
			token.erase(spaces[i], 1);
		}
	}

	void replace_all(std::string &str, const std::string &from, const std::string &to) {
		if (from.empty()) return;
		size_t start = 0;
		while ((start = str.find(from, start)) != std::string::npos) {
			str.replace(start, from.length(), to);
			start += to.length();
		}
	}

	std::string join(const std::vector<std::string> &Parts, const char *Delim) {
		std::ostringstream oss;
		for (size_t i = 0; i < Parts.size(); ++i) {
			if (i != 0) oss << Delim;
			oss << Parts[i];
		}
		return oss.str();
	}

	void trim(std::string &s) {
		size_t first = s.find_first_not_of(" \t");
		size_t last = s.find_last_not_of(" \t");
		if (first == std::string::npos)
			s.clear();
		else
			s = s.substr(first, last - first + 1);
	}

	int count_indent(const std::string &Line) {
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

}

bool gameplay_tag_generator::generate_from_env(const std::string &EnvVarName, const std::string &OutputUnit) {
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
	if (!parse(inputPath)) {
		return false;
	}
	return emit_files(inputPath, OutputUnit.empty() ? inputPath.stem().string() : OutputUnit);
}



bool gameplay_tag_generator::parse(const fs::path &FilePath) {
	std::ifstream in(FilePath);
	if (!in) {
		std::cerr << "Failed to open " << FilePath << std::endl;
		return false;
	}

	Root = std::make_unique<TagNode>();
	Root->Name = ""; // Dummy root

	std::stack<std::pair<int, std::shared_ptr<TagNode>>> indentStack; // pair<indentLevel, node>
	indentStack.emplace(-1, Root.get());

	std::string line;
	size_t lineNumber = 0;
	while (std::getline(in, line)) {
		++lineNumber;

		clean(line, lineNumber);

		int indent = count_indent(line);

		auto stripped = strip_indent(line);

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
			trim(comment);
		} else {
			token = stripped;
		}

		trim(token);

		if (token.empty()) {
			continue; // line was only a comment after indent
		}

		remove_spaces(token);

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



bool gameplay_tag_generator::emit_files(const fs::path &InputPath, const std::string &OutputUnit) {
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
	emit_node(Root, header, source, segments);

	std::cout << "Generated: \n  " << headerPath << "\n  " << sourcePath << std::endl;
	return true;
}

void gameplay_tag_generator::emit_node(const std::shared_ptr<TagNode> &Node, std::ofstream &Header, std::ofstream &Source,
                                    std::vector<std::string> &Segments) {
	if (Node.get() != Root.get()) // Skip dummy root
	{
		Segments.push_back(Node->Name);
		std::string underscoreName = join(Segments, "_");
		std::string dotName = join(Segments, ".");

		Header << "UE_DECLARE_GAMEPLAY_TAG_EXTERN(" << underscoreName << ")\n";

		// Escape double quotes in comments so we can safely embed them
		std::string escapedComment = Node->Comment;
		replace_all(escapedComment, "\"", "\\\"");
		Source << "UE_DEFINE_GAMEPLAY_TAG_COMMENT(" << underscoreName << ", \"" << dotName << "\", \"" <<
				escapedComment << "\")\n";
	}

	for (auto& child: Node->Children) {
		emit_node(child, Header, Source, Segments);
	}

	if (Node.get() != Root.get()) {
		Segments.pop_back();
	}
}

