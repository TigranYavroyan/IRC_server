#ifndef KICK_H
#define KICK_H

#include <ACommand.hpp>

class Kick : public ACommand {
public:
    Kick(IRCServer& server) : ACommand(server) {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // KICK_H