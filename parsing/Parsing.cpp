#include <Parsing.hpp>
#include <InputError.hpp>
#include <Helpers.hpp>
#include <cstdlib>

// for test
#include <iostream>

int Parsing::__validate_port (int port) {

}

std::string Parsing::__validate_password (const std::string& password) {

}

std::pair<int, std::string> Parsing::validate_input (int argc, char* argv[]) {
	if (argc != 3)
		throw IRC::InputError("Invalid count of argument");
	
	std::pair<int, std::string> res;

	res.first = std::atoi(argv[1]);
	res.second = Helpers::to_string(argv[2]);

	std::cout << res.first << ' ' << res.second << std::endl;
}

std::vector<std::string> Parsing::parse_msg (const std::string& msg) {

}

bool Parsing::is_valid_nickname (const std::string& nickname) {

}