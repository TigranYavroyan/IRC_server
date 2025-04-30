#include <UserTable.hpp>

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
		table_by_socket.insert({
			socket_fd,
			new_user
		});
		table_by_name.insert({
			nickname,
			new_user
		});
	}
	else {
		it->second->set(socket_fd, nickname, username, hostname, is_auth);
	}
}

UserTable::~UserTable () {
	std::map<int, User*>::iterator begin = table_by_socket.begin();
	std::map<int, User*>::iterator end = table_by_socket.end();

	while (begin != end) {
		delete begin->second;
		++begin;
	}
}