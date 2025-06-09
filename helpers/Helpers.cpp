#include <Helpers.hpp>

void Helpers::right_trim (std::string& str, const char* delims) {
    str.erase(str.find_last_not_of(delims) + 1);
}

void Helpers::left_trim (std::string& str, const char* delims) {
    str.erase(0, str.find_first_not_of(delims));
}

void Helpers::trim (std::string& str, const char* delims) {
    left_trim(str, delims);
    right_trim(str, delims);
}


std::vector<std::string> Helpers::parse_msg (const char* msg) {
	std::vector<std::string> tokens;
	std::string input(msg);
	std::string token;
	std::istringstream ss(input);

	while (ss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

std::vector<std::string> Helpers::parse_msg (const std::string& msg) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream ss(msg);

	while (ss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

std::string Helpers::merge_from(const std::vector<std::string>& tokens, size_t start) {
	std::string result;
	for (size_t i = start; i < tokens.size(); ++i) {
		if (i != start)
			result += " ";
		result += tokens[i];
	}
	if (!result.empty() && result[0] == ':')
		result.erase(0, 1);
	return result;
}