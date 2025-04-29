#ifndef PARSING_HPP
#define PARSING_HPP

#include <utility>
#include <string>
#include <vector>

class Parsing {
	static int __validate_port (int port);
	static std::string __validate_password (const std::string& password);
public:
	static std::pair<int, std::string> validate_input (int argc, char* argv[]);
	static std::vector<std::string> parse_msg (const std::string& msg);
	static bool is_valid_nickname (const std::string& nickname);
};

#endif // PARSING_HPP