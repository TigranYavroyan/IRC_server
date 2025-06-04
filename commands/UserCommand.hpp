#ifndef USER_COMMAND_HPP
#define USER_COMMAND_HPP

#include "ACommand.hpp"

class UserCommand : public ACommand {
public:
    UserCommand(IRCServer& server);
    virtual ~UserCommand();
    virtual void execute(int client_fd, const std::vector<std::string>& tokens);
private:
    std::string __merge_form(const std::vector<std::string>& tokens, size_t start);
};

#endif // USER_COMMAND_HPP
