#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <set>
#include <sys/socket.h>
#include <unistd.h>

class User {
	std::string username;
	std::string nickname;
	std::string hostname;
	std::string realname;
	int socket_fd;
	bool is_auth;
	bool is_registered;
	std::set<std::string> joined_channels;
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
   void set_is_registered (bool is_get = true);
   void exit_from_all_channels ();

   void join_channel (const std::string& channel_name);
   void exit_channel (const std::string& channel_name);
   bool is_in_channel (const std::string& channel_name) const;

   std::string get_username () const;
   std::string get_nickname () const;
   std::string get_hostname () const;
   std::string get_realname() const;
   std::set<std::string>::iterator ch_begin () const;
   std::set<std::string>::iterator ch_end () const;

   int get_socket_fd () const;
   bool get_is_auth () const;
   bool get_is_registered () const;

   bool is_nick () const;
   bool is_user () const;

   void sendMessage(const std::string& msg) const;
};

std::ostream& operator<< (std::ostream& os, const User& user);

#endif // USER_HPP