#ifndef PING_HPP
#define PING_HPP

#include "ACommand.hpp"

class Ping : public ACommand {
public:
    Ping(IRCServer& server);
    ~Ping();
    void execute(int client_fd, const std::vector<std::string>& tokens);
};

#endif
