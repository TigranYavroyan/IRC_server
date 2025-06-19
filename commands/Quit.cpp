#include <Quit.hpp>
#include <UserTable.hpp>
#include <IRCServer.hpp>
#include <Replies.hpp>
#include <Helpers.hpp>

void Quit::execute (int socket_fd, const std::vector<std::string>& tokens) {

	UserTable& usertable = server.getUserTable();
	User& user = usertable[socket_fd];
	std::string msg;

	if (tokens.size() == 1) {
		msg = Replies::quitMsg(user);
	}
	else {
		if (tokens[1][0] == ':')
			msg = Helpers::merge_from(tokens, 1);
		else
			msg = tokens[1];

		msg = Replies::quitMsg(user, msg);
	}

	server.removeFromAllChannels(user, msg);
	server.disconnectClient(user);
}
