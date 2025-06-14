#include <Mode.hpp>
#include <IRCServer.hpp>

void Mode::execute (int socket_fd, const std::vector<std::string>& tokens) {
	UserTable& usertable = server.getUserTable();
	User& user = usertable[socket_fd];
	std::string msg;

	if (tokens.size() == 1) {
		msg = Replies::err_notEnoughParam(tokens[0], user.get_nickname());
		user.sendMessage(msg);
		return;
	}

	if (tokens.size() == 2) {
		std::string channel_name = tokens[1];
		if (!server.is_channel_exist(channel_name)) {
			msg = Replies::err_noSuchChannel(user.get_nickname(), channel_name);
			user.sendMessage(msg);
		}
		else {
			/**
			 * $ Must send the channel mode with 324 reply number
			*/
			// msg = Replies::channelModes();
			// user.sendMessage(msg);
		}
		return;
	}

	
}