#include <Parsing.hpp>
#include <InputError.hpp>
#include <Helpers.hpp>
#include <cstdlib>
#include <cstring>
#include <errno.h>

int Parsing::__validate_port (char* port) {
	char* port_end = NULL;
	int res = std::strtol(port, &port_end, 10);

	if (*port_end != '\0' || res < PORT_NUMBER_LOWER_BOUND || res > PORT_NUMBER_UPPER_BOUND)
		throw IRC::InputError("The port must be number between [1024, 49152] interval");

	return res;
}

std::string Parsing::__validate_password (char* password) {
	if (std::strlen(password) >= MSG_SIZE)
		throw IRC::InputError("The password is to long (must be less than 512 charachters)");

	std::string res = Helpers::to_string(password);

	return res;
}

std::pair<int, std::string> Parsing::validate_input (int argc, char* argv[]) {
	if (argc != 3)
		throw IRC::InputError("Invalid count of argument");
	
	std::pair<int, std::string> res;

	res.first = __validate_port(argv[1]);
	res.second = __validate_password(argv[2]);

	return res;
}

std::vector<std::string> Parsing::parse_msg (const char* msg) {
	std::vector<std::string> tokens;
	std::string input(msg);
	std::string token;
	std::istringstream ss(input);

	while (ss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

std::vector<std::string> Parsing::parse_msg (const std::string& msg) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream ss(msg);

	while (ss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

bool Parsing::__is_special_char (char ch) {
	// [ \ ] ^ _ ` { | }
	return ((ch >= 91 && ch <= 96) || (ch >= 123 && ch <= 125));
}

bool Parsing::is_valid_nickname (const std::string& nickname) {
	if (nickname.size() > NICKNAME_SIZE)
		return false;
	
	if (!(std::isalpha(nickname[0]) || __is_special_char(nickname[0])))
		return false;
	
	for (std::size_t i = 0; i < nickname.size(); ++i) {
		if (!(std::isalnum(nickname[i]) || __is_special_char(nickname[i]) || nickname[i] == '-'))
			return false;
	}

	return true;
}