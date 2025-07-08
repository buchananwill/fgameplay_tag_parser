//
// Created by thele on 12/05/2025.
//

#ifndef HELPERS_H
#define HELPERS_H
#include <string>
#include <vector>

namespace helpers {
	void strip_carriage_return(std::string& line);

	void strip_bom(std::string& line, size_t lineNumber);

	std::string strip_indent(const std::string& line);

	void clean(std::string& line, size_t lineNumber);

	void remove_spaces(std::string& token);

	void replace_all(std::string& str, const std::string& match_string, const std::string& replace_with);

	std::string join(const std::vector<std::string>& Parts, const char* Delim);

	void trim(std::string& str);

	int count_indent(const std::string& line);
}

#endif //HELPERS_H
