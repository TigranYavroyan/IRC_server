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
		const std::string& username = "",
		const std::string& nickname = "",
		const std::string& hostname = "",
		bool is_auth = false
	);

	User get_user (int socket_fd);
	User get_user (const std::string& nickname);
	void remove_user (int socket_fd);
	void remove_user (const std::string& nickname);
};

#endif // USERTABLE_HPP