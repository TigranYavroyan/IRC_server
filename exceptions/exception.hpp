#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>
#include <exception>

namespace IRC {
	class exception : public std::exception {
	protected:
		std::string msg;
		exception() {}
	public:
		explicit exception (const char* _msg): msg(_msg ? _msg : "") {}
		explicit exception (const std::string& _msg): msg(_msg) {}
		exception (const exception& other) throw(): msg(other.msg) {}

		exception& operator= (const exception& other) {
			msg = other.msg;
		}

		virtual const char* what() const throw() {
			return msg.c_str();
		}
	};
}

#endif // EXCEPTION_HPP