//
// Created by thele on 12/05/2025.
//

#include "helpers.h"

#include <sstream>

void helpers::strip_carriage_return(std::string& line) {
	if (!line.empty() && line.back() == '\r')
		line.pop_back();
}

void helpers::strip_bom(std::string &line, size_t lineNumber) {
	if (lineNumber == 1 && line.rfind("\xEF\xBB\xBF", 0) == 0)
		line.erase(0, 3);
}

std::string helpers::strip_indent(const std::string &line) {
	const size_t first = line.find_first_not_of(" \t");
	return line.substr(first == std::string::npos ? line.size() : first);
}

void helpers::clean(std::string &line, size_t lineNumber) {
	strip_carriage_return(line);
	strip_bom(line, lineNumber);
}

void helpers::remove_spaces(std::string &token) {
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

void helpers::replace_all(std::string &str, const std::string &from, const std::string &to) {
	if (from.empty()) return;
	size_t start = 0;
	while ((start = str.find(from, start)) != std::string::npos) {
		str.replace(start, from.length(), to);
		start += to.length();
	}
}

std::string helpers::join(const std::vector<std::string> &Parts, const char *Delim) {
	std::ostringstream oss;
	for (size_t i = 0; i < Parts.size(); ++i) {
		if (i != 0) oss << Delim;
		oss << Parts[i];
	}
	return oss.str();
}

void helpers::trim(std::string &s) {
	size_t first = s.find_first_not_of(" \t");
	size_t last = s.find_last_not_of(" \t");
	if (first == std::string::npos)
		s.clear();
	else
		s = s.substr(first, last - first + 1);
}

int helpers::count_indent(const std::string &Line) {
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
