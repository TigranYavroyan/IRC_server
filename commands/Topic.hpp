#ifndef TOPIC_HPP
#define TOPIC_HPP

#include <ACommand.hpp>

class Topic : public ACommand {
public:
    Topic (IRCServer& server) : ACommand(server) {}
    virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // TOPIC_HPP