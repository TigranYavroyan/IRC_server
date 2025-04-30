#ifndef USER_HPP
#define USER_HPP

#include <string>

struct User {
	std::string username;
	std::string nickname;
	std::string hostname;
	int socket_fd;
	bool is_auth;
};

#endif // USER_HPP