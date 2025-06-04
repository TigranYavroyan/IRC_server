#include "Ping.hpp"
#include "Replies.hpp"
#include <sys/socket.h>

Ping::Ping(IRCServer& server) : ACommand(server) {}

Ping::~Ping() {}

void Ping::execute(int client_fd, const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::string msg = "ERROR :No origin specified" + Replies::crlf();
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }

    std::string response = "PONG " + tokens[1] + Replies::crlf();
    send(client_fd, response.c_str(), response.size(), 0);
}
