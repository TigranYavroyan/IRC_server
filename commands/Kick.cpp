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

    /**
     * 
     * 
     * if (channelNames[i][0] != '#') {
            msg = Replies::err_invalidChannelName(kicker.get_nickname(), channelNames[i]);
            kicker.sendMessage(msg);
            return;
        }
    */

    bool oneChannel = (channelNames.size() == 1);
	if (!oneChannel && channelNames.size() != userNames.size())
	{
        msg = Replies::err_notEnoughParam("KICK", kicker.get_nickname());
		kicker.sendMessage(msg);
		return;
	}

	for (size_t i = 0; i < userNames.size(); ++i)
	{
		std::string channelName = oneChannel ? channelNames[0] : channelNames[i];
		std::string targetNick = userNames[i];
		handleKick(kicker, channelName, targetNick, msg, server);
	}

    // if (channelNames.size() == 1) {
    //     std::string channelName = channelNames[0];

    //     if (!server.is_channel_exist(channelName)) {
    //         msg = Replies::err_noSuchChannel(kicker.get_nickname(), channelName);
    //         kicker.sendMessage(msg);
    //         return;
    //     }

    //     Channel& channel = server.getChannel(channelName);

    //     if (!channel.getUserByNick(kicker.get_nickname())) {
    //         msg = Replies::err_noOnThatChannel(kicker.get_nickname(), channelName);
    //         kicker.sendMessage(msg);
    //         return;
    //     }

    //     if (!channel.isOperator(&kicker)) {
    //         msg = Replies::err_notOperator(kicker.get_nickname(), channelName);
    //         kicker.sendMessage(msg);
    //         return;
    //     }

    //     for (std::size_t i = 0; i < userNames.size(); ++i) {
    //         std::string nick = userNames[i];

    //         if (!usertable.is_nickname_taken(nick)) {
    //             msg = Replies::err_noSuchNick("KICK", kicker.get_nickname());
    //             kicker.sendMessage(msg);
    //             continue;
    //         }

    //         if (!channel.getUserByNick(nick)) {
    //             msg = Replies::err_kickingNotInChannel(kicker.get_nickname(), nick, channelName);
    //             kicker.sendMessage(msg);
    //             continue;
    //         }

    //     }

    //     return;
    // }
}

void Kick::handleKick(User& client, const std::string &channelName, const std::string &targetNick, const std::string &msg, IRCServer& server) {

}