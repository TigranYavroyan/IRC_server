#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
	std::string username;
	std::string nickname;
	int socket_fd;
	std::string hostname; // ?
	bool is_auth;
public:
	// getters, setters
};

#endif // USER_HPP