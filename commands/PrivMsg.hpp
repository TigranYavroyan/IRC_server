#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include <ACommand.hpp>

class PrivMsg : public ACommand {
	void __bot_handle (const User& sender, std::string& msg) const;
public:
	PrivMsg (IRCServer& _server);
	virtual ~PrivMsg();
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // PRIVMSG_HPP
