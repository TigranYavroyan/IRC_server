#ifndef SERVERERROR_HPP
#define SERVERERROR_HPP

#include "exception.hpp"

namespace IRC {
	class ServerError : public IRC::exception {
	public:
		ServerError(const char* _msg);
		ServerError(const std::string& _msg);
		ServerError(const ServerError& other) throw();

		ServerError& operator= (const ServerError& other);
	};
}

#endif // SERVERERROR_HPP