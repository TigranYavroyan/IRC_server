#ifndef NICK_HPP
#define NICK_HPP

#include "ACommand.hpp"

class Nick : public ACommand
{
public:
    Nick(IRCServer& server);
    ~Nick();
    void execute(int client_socket, const std::vector<std::string>& tokens);
};

#endif // NICK_HPP
