#include "ServerError.hpp"

IRC::ServerError::ServerError(const char* _msg): exception(_msg) {}

IRC::ServerError::ServerError(const std::string& _msg): exception(_msg) {}

IRC::ServerError::ServerError(const ServerError& other) throw(): msg(other.msg) {}

IRC::ServerError& IRC::ServerError::operator= (const ServerError& other) {
	msg = other.msg;
}