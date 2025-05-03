#ifndef PART_HPP
#define PART_HPP

#include <ACommand.hpp>

class Part : public ACommand {
public:
	Part (IRCServer& _server) : ACommand(_server) {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // PART_HPP
