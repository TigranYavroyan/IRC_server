#ifndef INVITE_H
#define INVITE_H

#include <ACommand.hpp>

class Invite : public ACommand {
public:
    Invite(IRCServer& server): ACommand(server) {}  
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // INVITE_H