#include "Pong.hpp"

Pong::Pong(IRCServer& server) : ACommand(server) {}

Pong::~Pong() {}

void Pong::execute(int client_fd, const std::vector<std::string>& tokens) {
    (void)client_fd;
    (void)tokens;
}
