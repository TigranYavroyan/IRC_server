#ifndef PONG_HPP
#define PONG_HPP

#include "ACommand.hpp"

class Pong : public ACommand {
public:
    Pong(IRCServer& server);
    ~Pong();
    void execute(int client_fd, const std::vector<std::string>& tokens);
};

#endif
