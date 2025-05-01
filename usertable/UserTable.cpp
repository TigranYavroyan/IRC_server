#include <UserTable.hpp>
#include <ServerError.hpp>

UserTable::UserTable () {}

void UserTable::set_user (
	int socket_fd,
	const std::string& username,
	const std::string& nickname,
	const std::string& hostname,
	bool is_auth
)
{
	std::map<int, User*>::iterator it = table_by_socket.find(socket_fd);
	if (it == table_by_socket.end()) {
		User* new_user = new User(socket_fd, username, nickname, hostname, is_auth);
		table_by_socket.insert(
			std::make_pair(socket_fd, new_user)
		);
		table_by_name.insert(
			std::make_pair(nickname, new_user)
		);
	}
	else {
		it->second->set(socket_fd, nickname, username, hostname, is_auth);
	}
}

void UserTable::set_user (const User& user) {
	set_user(
		user.get_socket_fd(),
		user.get_username(),
		user.get_nickname(),
		user.get_hostname(),
		user.get_is_auth()
	);
}

UserTable::~UserTable () {
	std::map<int, User*>::iterator begin = table_by_socket.begin();
	std::map<int, User*>::iterator end = table_by_socket.end();

	while (begin != end) {
		delete begin->second;
		++begin;
	}
}

User UserTable::get_user (int socket_fd) const {
	std::map<int, User*>::const_iterator it = table_by_socket.find(socket_fd);
	if (it == table_by_socket.end())
		throw IRC::ServerError("The is no user with that socket_fd: " + socket_fd);
	
	return *(it->second);
}

User UserTable::get_user (const std::string& nickname) const {
	std::map<std::string, User*>::const_iterator it = table_by_name.find(nickname);
	if (it == table_by_name.end())
		throw IRC::ServerError("The is no user with that nicname: " + nickname);
	
	return *(it->second);
}

void UserTable::remove_user (int socket_fd) {
	std::map<int, User*>::iterator it = table_by_socket.find(socket_fd);
	if (it == table_by_socket.end())
		return;
	
	std::string nickname = it->second->get_nickname();
	delete it->second;
	table_by_socket.erase(it);
	table_by_name.erase(nickname);
}

void UserTable::remove_user (const std::string& nickname) {
	std::map<std::string, User*>::iterator it = table_by_name.find(nickname);
	if (it == table_by_name.end())
		return;
	
	int socket_fd = it->second->get_socket_fd();
	delete it->second;
	table_by_name.erase(it);
	table_by_socket.erase(socket_fd);
}