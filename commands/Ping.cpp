#include "Ping.hpp"
#include "Replies.hpp"
#include <sys/socket.h>
#include <IRCServer.hpp>

Ping::Ping(IRCServer& server) : ACommand(server) {}

Ping::~Ping() {}

void Ping::execute(int client_fd, const std::vector<std::string>& tokens) {
    User client = server.getUserTable().get_user(client_fd);
    if (tokens.size() < 2) {
        std::string msg = "ERROR :No origin specified" + Replies::crlf();
        client.sendMessage(msg);
        return;
    }

    std::string response = "PONG " + tokens[1] + Replies::crlf();
    client.sendMessage(response);
}
