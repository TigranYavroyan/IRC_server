#include <PrivMsg.hpp>
#include <IRCServer.hpp>

PrivMsg::PrivMsg (IRCServer& _server) : ACommand(_server) {}

PrivMsg::~PrivMsg () {}

void PrivMsg::execute (int socket_fd, const std::vector<std::string>& tokens) {

	UserTable& user_table = server.getUserTable();
	const User& user = user_table[socket_fd];
	std::string msg;

	if (tokens.size() < 3 || tokens[2][0] != ':') {
		msg = Replies::err_notEnoughParam("PRIVMSG", user.get_nickname());
        send(socket_fd, msg.c_str(), msg.size(), 0);
        return;
	}

	std::string recipients_name = tokens[1];
	msg = Helpers::merge_from(tokens, 2);

	if (recipients_name[0] == '#') {

		return;
	}

	if (!user_table.is_nickname_taken(recipients_name)) {
		msg = Replies::err_noSuchNick("PRIVMSG", "", recipients_name);
		send(user.get_socket_fd(), msg.c_str(), msg.size(), 0);
		return;
	}

	const User& recipient = user_table[recipients_name];
	msg = Replies::privateMessage(user, recipients_name, msg);
	send(recipient.get_socket_fd(), msg.c_str(), msg.size(), 0);
}
