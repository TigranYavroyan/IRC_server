#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include <Channel.hpp>
#include <vector>
#include <string>
#include <iostream>

class IRCServer;

class ACommand {
protected:
	IRCServer& server;
public:
	ACommand (IRCServer& _server): server(_server) {}
	virtual ~ACommand () {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens) = 0;
};

#endif // ACOMMAND_HPP