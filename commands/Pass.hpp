#ifndef PASS_HPP
#define PASS_HPP

#include <ACommand.hpp>

class Pass : public ACommand
{
    public:
        Pass(IRCServer& server);
        void execute(int socket_fd, const std::vector<std::string>& tokens);
    };
    
#endif // PASS_HPP