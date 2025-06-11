#include <PrivMsg.hpp>
#include <IRCServer.hpp>
#include <Channel.hpp>

PrivMsg::PrivMsg (IRCServer& _server) : ACommand(_server) {}

PrivMsg::~PrivMsg () {}

void PrivMsg::execute (int socket_fd, const std::vector<std::string>& tokens) {

	UserTable& user_table = server.getUserTable();
	const User& sender = user_table[socket_fd];
	std::string msg;

	if (tokens.size() == 1) {
		msg = Replies::err_noRecipientGiven(sender.get_nickname(), "PRIVMSG");
		sender.sendMessage(msg);
		return;
	}

	if (tokens.size() == 2) {
		msg = Replies::err_noTextToSend(sender.get_nickname());
		sender.sendMessage(msg);
		return;
	}

	if (tokens[2][0] != ':') {
		msg = Replies::err_notEnoughParam("PRIVMSG", sender.get_nickname());
		sender.sendMessage(msg);
        return;
	}

	std::string recipients_name = tokens[1];
	msg = Helpers::merge_from(tokens, 2);

	if (recipients_name[0] == '#') {

		if (!server.is_channel_exist(tokens[1])) {
			msg = Replies::err_noSuchNick(sender.get_nickname(), recipients_name);
			sender.sendMessage(msg);
			return;
		}

		Channel& ch = server.getChannel(recipients_name);
		msg = Replies::privateMessage(sender, recipients_name, msg);
		ch.broadcast(msg, &sender);
		return;
	}

	if (!user_table.is_nickname_taken(tokens[1])) {
		msg = Replies::err_noSuchNick(sender.get_nickname(), recipients_name);
		sender.sendMessage(msg);
		return;
	}

	const User& recipient = user_table[recipients_name];
	msg = Replies::privateMessage(sender, recipients_name, msg);
	recipient.sendMessage(msg);
}
