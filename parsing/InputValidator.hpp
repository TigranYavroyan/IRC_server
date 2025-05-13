#ifndef INPUTVALIDATOR_HPP
#define INPUTVALIDATOR_HPP

#include <utility>
#include <string>
#include <vector>

#include <Constants.hpp>

class InputValidator {
	static int __validate_port (char* port);
	static std::string __validate_password (char* password);
	static bool __is_special_char (char ch);
public:
	static std::pair<int, std::string> validate_input (int argc, char* argv[]);
	static std::vector<std::string> parse_msg (const char* msg);
	static std::vector<std::string> parse_msg (const std::string& msg);
	static bool is_valid_nickname (const std::string& nickname);
};

#endif // INPUTVALIDATOR_HPP