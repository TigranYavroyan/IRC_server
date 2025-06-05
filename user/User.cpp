#include "User.hpp"

User::User(int fd,
	const std::string& user,
	const std::string& nick,
	const std::string& host,
	const std::string& realname,
	bool auth)
	: username(user)
	, nickname(nick)
	, hostname(host)
	, realname(realname)
	, socket_fd(fd)
	, is_auth(auth)
	, has_user_info(false)
	, has_nick(false)
	, is_get_welcome_msg(false)
{}

void User::set(
	int fd,
	const std::string& nick,
	const std::string& user,
	const std::string& host,
	const std::string& realname,
	bool auth
)
{
	socket_fd = fd;
	nickname = nick;
	username = user;
	hostname = host;
	this->realname = realname;
	is_auth = auth;
}

void User::set(const User& user) {
	socket_fd = user.socket_fd;
	nickname = user.nickname;
	hostname = user.hostname;
	username = user.username;
	realname = user.realname;
	is_auth = user.is_auth;
	has_user_info = user.has_user_info;
	has_nick = user.has_nick;
}

void User::set_username(const std::string& user) {
	username = user;
}

void User::set_nickname(const std::string& nick) {
	nickname = nick;
}

void User::set_hostname(const std::string& host) {
	hostname = host;
}

void User::set_realname(const std::string& rn) {
	realname = rn;
}

void User::set_is_auth(bool auth) {
	is_auth = auth;
}

void User::set_socket_fd(int fd) {
	socket_fd = fd;
}

void User::set_has_user_info(bool value) {
	has_user_info = value;
}

void User::set_has_nick(bool value) {
	has_nick = value;
}

std::string User::get_username() const {
	return username;
}

std::string User::get_nickname() const {
	return nickname;
}

std::string User::get_hostname() const {
	return hostname;
}

std::string User::get_realname() const {
	return realname;
}

int User::get_socket_fd() const {
	return socket_fd;
}

bool User::get_is_auth() const {
	return is_auth;
}

bool User::get_has_user_info() const {
	return has_user_info;
}

bool User::get_has_nick() const {
	return has_nick;
}


void User::set_is_get_welcome_msg(bool is_get) {
	is_get_welcome_msg = is_get;
}

bool User::get_is_get_welcome_msg() const {
	return is_get_welcome_msg;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
	os << "socket_fd: " << user.get_socket_fd()
	   << "\nnickname: " << user.get_nickname()
	   << "\nusername: " << user.get_username()
	   << "\nhostname: " << user.get_hostname()
	   << "\nrealname: " << user.get_realname()
	   << "\nis_auth: " << user.get_is_auth()
	   << "\nhas_user_info: " << user.get_has_user_info()
	   << "\nhas_nick: " << user.get_has_nick();
	return os;
}

void User::sendMessage(const std::string& message) const {
	if (socket_fd >= 0) {
		std::string msg_with_newline = message + "\r\n";
		write(socket_fd, msg_with_newline.c_str(), msg_with_newline.size());
	}
}