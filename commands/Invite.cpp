#include <Invite.hpp>
#include <IRCServer.hpp>

void Invite::execute (int socket_fd, const std::vector<std::string>& tokens) {
    UserTable& usertable = server.getUserTable();
    User& sender = usertable[socket_fd];
    std::string msg;

    if (tokens.size() < 3) {
        msg = Replies::err_notEnoughParam("INVITE", sender.get_nickname());
        sender.sendMessage(msg);
        return;
    }

    std::string recipientName = tokens[1];
    std::string channelName = tokens[2];

    if (!usertable.is_nickname_taken(recipientName)) {
        msg = Replies::err_noSuchNick("INVITE", recipientName);
        sender.sendMessage(msg);
        return;
    }

    if (!server.is_channel_exist(channelName)) {
        msg = Replies::err_noSuchChannel(sender.get_nickname(), channelName);
        sender.sendMessage(msg);
        return;
    }

    Channel& channel = server.getChannel(channelName);

    if (!channel.getUserByNick(sender.get_nickname())) {
        msg = Replies::err_noOnThatChannel(sender.get_nickname(), channelName);
        sender.sendMessage(msg);
        return;
    }

    if (channel.getInviteOnly() && !channel.isOperator(&sender)) {
        msg = Replies::err_notOperator(sender.get_nickname(), channelName);
        sender.sendMessage(msg);
        return;
    }

    if (channel.getUserByNick(recipientName)) {
        msg = Replies::err_alreadyInChannel(sender.get_nickname(), recipientName, channelName);
        sender.sendMessage(msg);
        return;
    }

    msg = Replies::inviteMsgToSender(sender.get_nickname(), recipientName, channelName);
    sender.sendMessage(msg);
    msg = Replies::inviteMsg(sender, recipientName, channelName);
    User& recipient = usertable[recipientName];
    recipient.sendMessage(msg);
    channel.inviteUser(&recipient);
}