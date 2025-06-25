#ifndef USERTABLE_HPP
#define USERTABLE_HPP

#include <map>
#include <User.hpp>
#include <Helpers.hpp>
#include <string>
#include <aliases.hpp>

class UserTable {
	UserByName table_by_name;
	UserBySocket table_by_socket;
public:
	UserTable ();
	~UserTable ();
public:
	void set_user (
		int socket_fd,
		const std::string& hostname = "",
		const std::string& nickname = "*",
		const std::string& username = "",
		const std::string& realname = "",
		bool is_auth = false
	);
	void set_user (const User& user);
	void set_user_nickname (int socket_fd, const std::string& nickname);
	void set_user_username (int socket_fd, const std::string& username);
	void set_user_hostname (int socket_fd, const std::string& hostname);
	void set_user_realname (int socket_fd, const std::string& realname);
	void set_user_auth (int socket_fd, bool auth = true);

	bool is_nickname_taken (const std::string& nickname) const;

	UserBySocketIter tsbegin();
	UserBySocketIter tsend();
	UserByNameIter tnbegin();
	UserByNameIter tnend();

	User get_user (int socket_fd) const;
	User get_user (const std::string& nickname) const;

	User& operator[] (int socket_fd);
	User& operator[] (const std::string& nickname);
	
	void remove_user (int socket_fd);
	void remove_user (const std::string& nickname);
};

#endif // USERTABLE_HPP