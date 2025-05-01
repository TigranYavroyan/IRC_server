#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <ostream>

class User {
	std::string username;
	std::string nickname;
	std::string hostname;
	int socket_fd;
	bool is_auth;
public:
	User(int fd,
		const std::string& user = "",
		const std::string& nick = "",
		const std::string& host = "",
		bool auth = false);

   void set(
	int fd,
	const std::string& nick,
	const std::string& user,
	const std::string& host,
	bool auth
   );

   void set(const User& user);

   std::string get_username () const;
   std::string get_nickname () const;
   std::string get_hostname () const;
   int get_socket_fd () const;
   bool get_is_auth () const;
};

std::ostream& operator<< (std::ostream& os, const User& user);

#endif // USER_HPP