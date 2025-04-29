#ifndef PARSING_HPP
#define PARSING_HPP

#include <utility>
#include <string>
#include <vector>

class Parsing {
	static int validate_port (int port);
	static std::string validate_password (std::string);
public:
	static std::pair<int, std::string> validate_input (int argc, char* argv[]);
	static std::vector<std::string> parse_msg (const std::string& msg);
	static bool is_valid_nickname (const std::string& nickname);
};

#endif // PARSING_HPP