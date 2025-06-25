#include "Ping.hpp"
#include "Replies.hpp"
#include <sys/socket.h>
#include <IRCServer.hpp>

Ping::Ping(IRCServer& server) : ACommand(server) {}

Ping::~Ping() {}

void Ping::execute(int client_fd, const std::vector<std::string>& tokens) {
    User client = server.getUserTable().get_user(client_fd);
    std::string msg;

    if (tokens.size() < 2) {
        msg = Replies::err_noOrigin(client.get_nickname());
        client.sendMessage(msg);
        return;
    }


    if (tokens[1][0] == ':') {
        msg = Helpers::merge_from(tokens, 1);
    }
    else {
        msg = tokens[1];
    }

	msg = Replies::pongMsg(msg);
    client.sendMessage(msg);
}
