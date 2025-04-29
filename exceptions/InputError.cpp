#include <InputError.hpp>

IRC::InputError::InputError(const char* _msg): exception(_msg) {}

IRC::InputError::InputError(const std::string& _msg): exception(_msg) {}

IRC::InputError::InputError(const InputError& other) throw(): exception(other.msg) {}

IRC::InputError& IRC::InputError::operator= (const InputError& other) {
	msg = other.msg;
	return *this;
}