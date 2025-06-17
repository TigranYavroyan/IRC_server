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
}