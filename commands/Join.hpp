#ifndef JOIN_HPP
#define JOIN_HPP

#include "ACommand.hpp"

class Join : public ACommand {
public:
    Join(IRCServer& server);
    void execute(int socket_fd, const std::vector<std::string>& tokens);
    std::vector<std::string> ft_split(const std::string& str, char delimiter);
};

#endif

