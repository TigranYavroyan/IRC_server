#include <InputValidator.hpp>
#include <InputError.hpp>
#include <Helpers.hpp>
#include <cstdlib>
#include <cstring>
#include <cerrno>

int InputValidator::__validate_port (char* port) {
	char* port_end = NULL;
	errno = 0;
	int res = std::strtol(port, &port_end, 10);

	if (*port_end != '\0' || res < PORT_NUMBER_LOWER_BOUND || res > PORT_NUMBER_UPPER_BOUND || errno != 0)
		throw IRC::InputError("The port must be number between [1024, 49152] interval");

	return res;
}

std::string InputValidator::__validate_password (char* password) {
	if (std::strlen(password) >= MSG_SIZE)
		throw IRC::InputError("The password is to long (must be less than 512 charachters)");

	std::string res = Helpers::to_string(password);

	return res;
}

std::pair<int, std::string> InputValidator::validate_input (int argc, char* argv[]) {
	if (argc != 3)
		throw IRC::InputError("Invalid count of argument");
	
	std::pair<int, std::string> res;

	res.first = __validate_port(argv[1]);
	res.second = __validate_password(argv[2]);

	return res;
}

bool InputValidator::__is_special_char (char ch) {
	// [ \ ] ^ _ ` { | }
	return ((ch >= 91 && ch <= 96) || (ch >= 123 && ch <= 125));
}

bool InputValidator::is_valid_nickname (const std::string& nickname) {
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