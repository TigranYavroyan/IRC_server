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

		if (!ch.getUserByNick(sender.get_nickname())) {
	        msg = Replies::err_noOnThatChannel(sender.get_nickname(), ch.getName());
			sender.sendMessage(msg);
			return;
		}

		msg = Replies::privateMessage(sender, recipients_name, msg);
		ch.broadcast(msg, &sender);
		return;
	}

	if (tokens[1] == "bot") {
		__bot_handle(sender, msg);
		return;
	}

	if (!user_table.is_nickname_taken(tokens[1])) {
		msg = Replies::err_noSuchNick(sender.get_nickname(), recipients_name);
		sender.sendMessage(msg);
		return;
	}

	const User& recipient = user_table[recipients_name];

	if (msg.find("\x01""DCC SEND") != std::string::npos) {
		msg = Replies::userFullName(sender) + " PRIVMSG " + recipients_name + " :" + msg;
		std::cout << "[DEBUG] sended message: " << msg << std::endl;
	} else {
		msg = Replies::privateMessage(sender, recipients_name, msg);
	}
	
	recipient.sendMessage(msg);
}

void PrivMsg::__bot_handle (const User& sender, std::string& msg) const {
	std::string bot_answer;
	User bot(-1, "bot", "bot", "bot", "bot", true);

	if (msg == "hello") {
		bot_answer = "Hello my friend, welcome the irc.42.chat :)";
		msg = Replies::privateMessage(bot, sender.get_nickname(), bot_answer);
		sender.sendMessage(msg);
	}
	else {
		bot_answer = "Sorry, I don't understand. You can only say \"hello\" to me :)";
		msg = Replies::privateMessage(bot, sender.get_nickname(), bot_answer);
		sender.sendMessage(msg);
	}
}