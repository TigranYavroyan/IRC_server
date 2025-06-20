#ifndef MODE_HPP
#define MODE_HPP

#include <ACommand.hpp>

class Mode : public ACommand {
private:
	void __mode_set (const ModeChange& cmd, Channel& channel);
	void __mode_remove (const ModeChange& cmd, Channel& channel);
	std::string __format_mode_reply (const std::string& channel, const std::vector<ModeChange>& operations);
public:
	Mode (IRCServer& server) : ACommand(server) {}
	virtual void execute (int socket_fd, const std::vector<std::string>& tokens);
};

#endif // MODE_HPP