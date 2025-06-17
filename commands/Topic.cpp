#include <Topic.hpp>
#include <IRCServer.hpp>

void Topic::execute (int socket_fd, const std::vector<std::string>& tokens) {
    UserTable& usertable = server.getUserTable();
    User& user = usertable[socket_fd];
    std::string msg;
    std::string channelName;
    
    if (tokens.size() == 1) {
        msg = Replies::err_notEnoughParam("TOPIC", user.get_nickname());
        user.sendMessage(msg);
        return;
    }

    channelName = tokens[1];

    if (!server.is_channel_exist(channelName)) {
        msg = Replies::err_noSuchChannel(user.get_nickname(), channelName);
        user.sendMessage(msg);
        return;
    }

    Channel& channel = server.getChannel(channelName);

    if (tokens.size() == 2) {
        std::string topic = channel.getTopic();

        if (topic.empty()) {
            msg = Replies::err_noTopicSet(user.get_nickname(), channelName);
        }
        else {
            msg = Replies::topicIs(user.get_nickname(), channelName, channel.getTopic());
        }
        user.sendMessage(msg);
        return;
    }

    if (tokens[2][0] != ':') {
        msg = Replies::err_notEnoughParam("TOPIC", user.get_nickname());
        user.sendMessage(msg);
        return;
    }

    if (channel.getTopicRestricted() && !channel.isOperator(&user)) {
        msg = Replies::err_notOperator(user.get_nickname(), channelName);
        user.sendMessage(msg);
        return;
    }

    std::string new_topic = Helpers::merge_from(tokens, 2);
    channel.setTopic(new_topic);
    msg = Replies::topicMsg(user, channelName, new_topic);
    channel.broadcast(msg);
}