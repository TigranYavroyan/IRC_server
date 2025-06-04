#include "Pass.hpp"
#include "IRCServer.hpp"

Pass::Pass(IRCServer& server) : ACommand(server) {}

void Pass::execute(int socket_fd, const std::vector<std::string>& tokens) {
    User client = server.getUserTable().get_user(socket_fd);
    if (tokens.size() < 2)
    {
        std::string error_msg = Replies::err_notEnoughParam("PASS", client.get_nickname());
        send(socket_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    const std::string& server_password = server.getPassword();
    if (tokens[1] == server_password)
    {
        // std::string success_msg = "Password accepted. Welcome!\n";
        // send(socket_fd, success_msg.c_str(), success_msg.size(), 0);
        server.getUserTable().set_user_auth(socket_fd);
    }
    else
    {
        std::string error_msg = Replies::err_incorpass(client.get_nickname());
        send(socket_fd, error_msg.c_str(), error_msg.size(), 0);
    }
}
