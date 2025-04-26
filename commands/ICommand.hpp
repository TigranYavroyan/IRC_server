#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <Channel.hpp>
#include <vector>
#include <string>

class ICommand {
public:
	virtual void execute (int socket_fd, const Channel& channel, std::vector<std::string> user_input) = 0;
};

#endif // ICOMMAND_HPP