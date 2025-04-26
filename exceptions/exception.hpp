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
		explicit exception (const char* _msg);
		explicit exception (const std::string& _msg);
		exception (const exception& other) throw();

		exception& operator= (const exception& other);

		virtual const char* what() const throw();
	};
}

#endif // EXCEPTION_HPP