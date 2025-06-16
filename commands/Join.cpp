#include <Join.hpp>
#include <Replies.hpp>
#include <Channel.hpp>
#include <UserTable.hpp>
#include <IRCServer.hpp>
#include <sstream>
#include <Logger.hpp>

Join::Join(IRCServer& server) : ACommand::ACommand(server) {}

void Join::execute(int client_fd, const std::vector<std::string>& tokens) {
    UserTable& usertable = server.getUserTable();
    User& user = usertable[client_fd];
    std::string msg;

    // if (!user.get_is_auth()) {
    //     user.sendMessage(Replies::err_notRegistered("JOIN", user.get_nickname()));
    //     return;
    // }

    if (tokens.size() < 2) {
        user.sendMessage(Replies::err_notEnoughParam("JOIN", user.get_nickname()));
        return;
    }

    std::vector<std::string> channelNames = Helpers::split_by_delim(tokens[1], ',');
    for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it) {
        std::string channelName = *it;

        if (channelName.empty() || channelName[0] != '#') {
            user.sendMessage(Replies::err_cannotJoin(user.get_nickname(), channelName));
            continue;
        }
        
        Channel& channel = server.getChannel(channelName);
        if (!channel.addUser(&user, msg)) {
            user.sendMessage(msg);
            continue;
        }
        
        Logger::client_join(user.get_nickname(), channelName);
        std::string joinMsg = Replies::joinMsg(user, channelName);
        channel.broadcast(joinMsg);
        
        user.join_channel(channelName);

        if (!channel.getTopic().empty()) {
            user.sendMessage(Replies::topicIs(user.get_nickname(), channelName, channel.getTopic()));
        }

        // Send NAMES list
        user.sendMessage(Replies::namReply(user.get_nickname(), channelName, channel.getUserList()));
        user.sendMessage(Replies::endOfNames(user.get_nickname(), channelName));
    }
}