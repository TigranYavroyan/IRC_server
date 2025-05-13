#include "Pass.hpp"
#include "IRCServer.hpp"

Pass::Pass(IRCServer& server) : ACommand(server) {}

void Pass::execute(int socket_fd, const std::vector<std::string>& tokens) {
    if (tokens.size() < 2)
    {
        std::string error_msg = "ERROR: Password required.\n";
        send(socket_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    const std::string& server_password = server.getPassword();
    if (tokens[1] == server_password)
    {
        std::string success_msg = "Password accepted. Welcome!\n";
        send(socket_fd, success_msg.c_str(), success_msg.size(), 0);
    }
    else
    {
        std::string error_msg = "ERROR: Incorrect password.\n";
        send(socket_fd, error_msg.c_str(), error_msg.size(), 0);
    }
}
