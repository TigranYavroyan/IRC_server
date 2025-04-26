#include "exception.hpp"

IRC::exception::exception(const char* _msg): msg(_msg ? _msg : "") {}

IRC::exception::exception(const std::string& _msg): msg(_msg) {}

IRC::exception::exception(const exception& other) throw(): msg(other.msg) {}

IRC::exception& IRC::exception::operator= (const exception& other) {
	msg = other.msg;
}

const char* IRC::exception::what() const throw() {
	return msg.c_str();
}