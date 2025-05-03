#ifndef QUIT_HPP
#define QUIT_HPP

#include <ACommand.hpp>

class Quit : public ACommand {
public:
	Quit (IRCServer& _server) : ACommand(_server) {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // QUIT_HPP
