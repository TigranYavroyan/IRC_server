#ifndef NICK_HPP
#define NICK_HPP

#include <ACommand.hpp>

class Nick : public ACommand {
public:
	Nick (IRCServer& _server) : ACommand(_server) {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // NICK_HPP
