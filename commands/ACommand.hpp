#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include <Channel.hpp>
#include <vector>
#include <string>
#include <IRCServer.hpp>

class ACommand {
protected:
	IRCServer& server;
public:
	ACommand (IRCServer& _server): server(_server) {}
	virtual ~ACommand () {}
	virtual void execute (int socket_fd, std::vector<std::string> user_input) = 0;
};

#endif // ACOMMAND_HPP