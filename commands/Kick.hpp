#ifndef KICK_H
#define KICK_H

#include <ACommand.hpp>

class Kick : public ACommand {
private:
    void handleKick(User& client, const std::string &channelName, const std::string &targetNick, const std::string &msg, IRCServer& server);
public:
    Kick(IRCServer& server) : ACommand(server) {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // KICK_H