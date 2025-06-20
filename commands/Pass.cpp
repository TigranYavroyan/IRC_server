#include "Pass.hpp"
#include "IRCServer.hpp"

Pass::Pass(IRCServer& server) : ACommand(server) {}

void Pass::execute(int socket_fd, const std::vector<std::string>& tokens) {
    User client = server.getUserTable().get_user(socket_fd);
    if (tokens.size() < 2)
    {
        std::string error_msg = Replies::err_notEnoughParam("PASS", client.get_nickname());
        client.sendMessage(error_msg);
        return;
    }

    const std::string& server_password = server.getPassword();
    if (tokens[1] == server_password)
    {
        server.getUserTable().set_user_auth(socket_fd);
    }
    else
    {
        std::string error_msg = Replies::err_incorpass("PASS", client.get_nickname());
        client.sendMessage(error_msg);
    }
}
