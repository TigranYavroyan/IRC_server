#ifndef MODE_HPP
#define MODE_HPP

#include <ACommand.hpp>

class Mode : public ACommand {
public:
	Mode (IRCServer& server) : ACommand(server) {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // MODE_HPP