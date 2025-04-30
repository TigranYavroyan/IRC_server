#include <User.hpp>

User::User(int fd,
	const std::string& user,
	const std::string& nick,
	const std::string& host,
	bool auth)
 : socket_fd(fd)
 , username(user)
 , nickname(nick)
 , hostname(host)
 , is_auth(auth)
{}

void User::set(
	int fd,
	const std::string& nick,
	const std::string& user,
	const std::string& host,
	bool auth
)
{
	socket_fd = fd;
	nickname = nick;
	username = user;
	hostname = host;
	is_auth = auth;
}

std::string User::get_username () const{
	return username;
}

std::string User::get_nickname () const{
	return nickname;
}

std::string User::get_hostname () const{
	return hostname;
}

int User::get_socket_fd () const{
	return socket_fd;
}

bool User::get_is_auth () const{
	return is_auth;
}