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

std::vector<std::string> Parsing::parse_msg (const std::string& msg) {

}

bool Parsing::is_valid_nickname (const std::string& nickname) {

}