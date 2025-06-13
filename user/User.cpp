#include <User.hpp>
#include <Replies.hpp>


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
	, is_registered(false)
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
	is_registered = user.is_registered;
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


void User::set_is_registered(bool is_get) {
	is_registered = is_get;
}

bool User::get_is_registered() const {
	return is_registered;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
	os << "socket_fd: " << user.get_socket_fd()
	   << "\nnickname: " << user.get_nickname()
	   << "\nusername: " << user.get_username()
	   << "\nhostname: " << user.get_hostname()
	   << "\nrealname: " << user.get_realname()
	   << "\nis_auth: " << user.get_is_auth();
	return os;
}

bool User::is_nick () const {
	return nickname.empty();
}

bool User::is_user () const {
	return username.empty();
}

void User::sendMessage(const std::string& msg) const {
	if (socket_fd >= 0) {
		send(socket_fd, msg.c_str(), msg.size(), 0);
	}
}

void User::join_channel (const std::string& channel_name) {
	joined_channels.insert(channel_name);
}

void User::exit_channel(const std::string& channel_name) {
	joined_channels.erase(channel_name);
}

bool User::is_in_channel (const std::string& channel_name) const {
	return joined_channels.count(channel_name);
}