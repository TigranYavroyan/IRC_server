#ifndef JOIN_HPP
#define JOIN_HPP

#include <ACommand.hpp>

class Join : public ACommand {
public:
	Join (IRCServer& _server) : ACommand(_server) {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // JOIN_HPP