#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
	std::string username;
	std::string nickname;
	std::string hostname;
	int socket_fd;
	bool is_auth;
public:
	// getters, setters
};

#endif // USER_HPP