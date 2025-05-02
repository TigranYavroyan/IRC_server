#ifndef USERTABLE_HPP
#define USERTABLE_HPP

#include <map>
#include <User.hpp>
#include <string>

class UserTable {
	std::map<std::string, User*> table_by_name;
	std::map<int, User*> table_by_socket;
public:
	UserTable ();
	~UserTable ();
public:
	void set_user (
		int socket_fd,
		const std::string& nickname = "",
		const std::string& username = "",
		const std::string& hostname = "",
		bool is_auth = false
	);
	void set_user (const User& user);

	void set_user_nickname (int socket_fd, const std::string& nickname);
	void set_user_username (int socket_fd, const std::string& username);
	void set_user_hostname (int socket_fd, const std::string& hostname);

	User get_user (int socket_fd) const;
	User get_user (const std::string& nickname) const;
	
	void remove_user (int socket_fd);
	void remove_user (const std::string& nickname);
};

#endif // USERTABLE_HPP