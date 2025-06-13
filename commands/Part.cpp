#include <Part.hpp>
#include <UserTable.hpp>
#include <IRCServer.hpp>
#include <User.hpp>

void Part::execute (int socket_fd, const std::vector<std::string>& tokens) {

	UserTable& usertable = server.getUserTable();
	User& user = usertable[socket_fd];
	std::string part_msg = "";
	std::string msg;
	std::string channelName;
	std::vector<std::string> channels;

	if (tokens.size() == 1) {
		msg = Replies::err_notEnoughParam(tokens[0], user.get_nickname());
		user.sendMessage(msg);
		return;
	}

	if (tokens.size() > 2) {
		if (tokens[2][0] == ':') {
			part_msg = ":" + Helpers::merge_from(tokens, 2);
		}
		else {
			part_msg = ":" + tokens[2];
		}
	}

	channels = Helpers::split_by_delim(tokens[1], ',');

	for (std::size_t i = 0; i < channels.size(); ++i) {
		channelName = channels[i];

		if (!server.is_channel_exist(channelName)) {
			msg = Replies::err_noSuchChannel(user.get_nickname(), channelName);
			user.sendMessage(msg);
			continue;
		}

		if (!user.is_in_channel(channelName)) {
			msg = Replies::err_noOnThatChannel(user.get_nickname(), channelName);
			user.sendMessage(msg);
			continue;
		}

		msg = Replies::partMsg(user, channelName, part_msg);
		Channel& ch = server.getChannel(channelName);
		ch.broadcast(msg, &user);
		ch.removeOperator(&user);
		ch.removeUser(&user);
		user.exit_channel(channelName);
	}
}
