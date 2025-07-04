#include <Join.hpp>
#include <Replies.hpp>
#include <Channel.hpp>
#include <UserTable.hpp>
#include <IRCServer.hpp>
#include <sstream>
#include <Logger.hpp>

#ifdef DEBUG
    #include <Debugger.hpp>
#endif

Join::Join(IRCServer& server) : ACommand::ACommand(server) {}

void Join::execute(int client_fd, const std::vector<std::string>& tokens) {
    UserTable& usertable = server.getUserTable();
    User& user = usertable[client_fd];
    std::string msg;

    if (tokens.size() < 2) {
        user.sendMessage(Replies::err_notEnoughParam("JOIN", user.get_nickname()));
        return;
    }

    if (tokens.size() == 2 && tokens[1] == "0") {
        server.removeFromAllChannels(user);
        return;
    }

    std::vector<std::string> channelNames = Helpers::split_by_delim(tokens[1], ',');
    std::vector<std::string> channelKeys;
    try {
        channelKeys = Helpers::split_by_delim(tokens.at(2), ',');
    }
    catch (const std::out_of_range& ex) {
        
        #ifdef DEBUG
            Debugger::exception_msg(ex);
        #endif

        channelKeys = std::vector<std::string>();
    }
    std::string channelName;
    std::string channelKey;

    for (std::size_t i = 0; i < channelNames.size(); ++i) {
        channelName = channelNames[i];

        try {
            channelKey = channelKeys.at(i);
        }
        catch (const std::out_of_range& ex) {

            #ifdef DEBUG
                Debugger::exception_msg(ex);
            #endif

            channelKey = "";
        }

        if (channelName.empty() || channelName[0] != '#') {
            user.sendMessage(Replies::err_cannotJoin(user.get_nickname(), channelName));
            continue;
        }

        Channel& channel = server.getChannel(channelName);

        if (channel.getUserByNick(user.get_nickname())) {
            continue;
        }

        if (!channel.addUser(&user, msg, channelKey)) {
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
        user.sendMessage(Replies::channelModes(user.get_nickname(), channelName, channel.getModes()));
        user.sendMessage(Replies::namReply(user.get_nickname(), channelName, channel.getUserList()));
        user.sendMessage(Replies::endOfNames(user.get_nickname(), channelName));
    }
}