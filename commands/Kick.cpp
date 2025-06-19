#include <Kick.hpp>
#include <IRCServer.hpp>


/**
 * ! Valid cases
 * 
 * $ KICK #room <userlist> :<optionalmsg>
 * $ KICK <channellist> user :<optionalmsg>
 * $ KICK #room1,#room2,#room3 user1,user2,user3 :<optionalmsg>
 * 
 * ! Invalid cases
 * 
 * $ KICK #room1,#room2 user1,user2,user3
 * $ KICK #room
 * $ and existens of users, channels and privilegies
*/

void Kick::execute (int socket_fd, const std::vector<std::string>& tokens) {
    UserTable& usertable = server.getUserTable();
    User& kicker = usertable[socket_fd];
    std::string msg;

    if (tokens.size() < 3) {
        msg = Replies::err_notEnoughParam("KICK", kicker.get_nickname());
        kicker.sendMessage(msg);
        return;
    }

    std::vector<std::string> channelNames = Helpers::split_by_delim(tokens[1], ',');
    std::vector<std::string> userNames = Helpers::split_by_delim(tokens[2], ',');

    bool oneChannel = (channelNames.size() == 1);

	if (!oneChannel && channelNames.size() != userNames.size())
	{
        msg = Replies::err_notEnoughParam("KICK", kicker.get_nickname());
		kicker.sendMessage(msg);
		return;
	}

    if (oneChannel && channelNames[0][0] != '#') {
        msg = Replies::err_invalidChannelName(kicker.get_nickname(), channelNames[0]);
        kicker.sendMessage(msg);
        return;
    }
    
    if (oneChannel && !server.is_channel_exist(channelNames[0])) {
        msg = Replies::err_noSuchChannel(kicker.get_nickname(), channelNames[0]);
        kicker.sendMessage(msg);
        return;
    }

    if (tokens.size() == 4) {
        if (tokens[3][0] == ':') {
			msg = Helpers::merge_from(tokens, 3);
		}
		else {
			msg = tokens[3];
		}
    }

	for (size_t i = 0; i < userNames.size(); ++i)
	{
		std::string channelName = oneChannel ? channelNames[0] : channelNames[i];
		std::string targetNick = userNames[i];
		handleKick(kicker, channelName, targetNick, msg);
	}
}

void Kick::handleKick(User& kicker, const std::string &channelName, const std::string& targetNick, const std::string& kickMsg) {
    std::string msg;

    if (channelName[0] != '#') {
        msg = Replies::err_invalidChannelName(kicker.get_nickname(), channelName);
        kicker.sendMessage(msg);
        return;
    }
    
    if (!server.is_channel_exist(channelName)) {
        msg = Replies::err_noSuchChannel(kicker.get_nickname(), channelName);
        kicker.sendMessage(msg);
        return;
    }

    Channel& channel = server.getChannel(channelName);
    UserTable& usertable = server.getUserTable();

    if (!usertable.is_nickname_taken(targetNick)) {
        msg = Replies::err_noSuchNick("KICK", targetNick); // maybe argument may change
        kicker.sendMessage(msg);
        return;
    }

    if (!channel.getUserByNick(targetNick)) {
        msg = Replies::err_recipientNotInChannel(kicker.get_nickname(), targetNick, channelName);
        kicker.sendMessage(msg);
        return;
    }

    if (!channel.getUserByNick(kicker.get_nickname())) {
        msg = Replies::err_noOnThatChannel(kicker.get_nickname(), channelName);
        kicker.sendMessage(msg);
        return;
    }

    if (!channel.isOperator(&kicker)) {
        msg = Replies::err_notOperator(kicker.get_nickname(), channelName);
        kicker.sendMessage(msg);
        return;
    }

    User& targetUser = usertable[targetNick];
    msg = Replies::kickMsg(kicker, channelName, targetNick, kickMsg);
    channel.broadcast(msg);
    channel.removeOperator(&targetUser);
    channel.removeUser(&targetUser);
    targetUser.exit_channel(channelName);

    if (!channel.getUserCount()) {
        server.removeChannel(channelName);
    }
}