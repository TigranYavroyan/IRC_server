#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

class User {
	std::string username;
	std::string nickname;
	std::string hostname;
	std::string realname;
	int socket_fd;
	bool is_auth;
	bool has_user_info;
	bool has_nick; 
public:
	User(int fd,
		const std::string& user = "",
		const std::string& nick = "",
		const std::string& host = "",
		const std::string& real = "",
		bool auth = false);
public:
   	void set(
	int fd,
	const std::string& nick,
	const std::string& user,
	const std::string& host,
	const std::string& real,
	bool auth
   );

   void set(const User& user);

   void set_username (const std::string& user);
   void set_nickname (const std::string& nick);
   void set_hostname (const std::string& host);
   void set_realname(const std::string& realname);
   void set_is_auth (bool auth = true);
   void set_socket_fd (int fd);

   std::string get_username () const;
   std::string get_nickname () const;
   std::string get_hostname () const;
   std::string get_realname() const;
   int get_socket_fd () const;
   bool get_is_auth () const;

   void set_has_user_info(bool value);
   void set_has_nick(bool value);
   bool get_has_user_info() const;
   bool get_has_nick() const;

   void sendMessage(const std::string& msg) const;
};

std::ostream& operator<< (std::ostream& os, const User& user);

#endif // USER_HPP