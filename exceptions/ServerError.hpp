#ifndef SERVERERROR_HPP
#define SERVERERROR_HPP

#include "exception.hpp"

namespace IRC {
	class ServerError : public IRC::exception {
	public:
		ServerError(const char* _msg): IRC::exception(_msg) {}
		ServerError(const std::string& _msg): IRC::exception(_msg.c_str()) {}
		ServerError(const ServerError& other) {
			this->msg = other.msg;
		}

		ServerError& operator= (const ServerError& other) {
			this->msg = other.msg;
		}
	};
}

#endif // SERVERERROR_HPP