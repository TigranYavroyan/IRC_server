#ifndef INPUTERROR_HPP
#define INPUTERROR_HPP

#include <exception.hpp>
#include <string>

namespace IRC {
	class InputError : public IRC::exception {
	public:
		InputError(const char* _msg);
		InputError(const std::string& _msg);
		InputError(const InputError& other) throw();

		InputError& operator= (const InputError& other);
	};
}

#endif // INPUTERROR_HPP