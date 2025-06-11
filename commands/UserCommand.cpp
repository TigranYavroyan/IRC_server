#include "UserCommand.hpp"
#include "User.hpp"
#include "IRCServer.hpp"
#include <cstring>
#include <Replies.hpp>

UserCommand::UserCommand(IRCServer& server) : ACommand(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(int client_fd, const std::vector<std::string>& tokens) {

    // need to get the reference , not copy to affect the change of username
    UserTable& user_table = server.getUserTable();
    User client = user_table.get_user(client_fd);

    if (!client.is_user()) {
        std::string msg = Replies::err_alreadyRegistered("USER", client.get_nickname());
        client.sendMessage(msg);
        return;
    }

    if (tokens.size() < 5 || tokens[4][0] != ':') {
        std::string msg = Replies::err_notEnoughParam("USER", client.get_nickname());
        client.sendMessage(msg);
        return;
    }

    std::string username = tokens[1];
    std::string realname = Helpers::merge_from(tokens, 4);

    user_table.set_user_username(client_fd, username);
    user_table.set_user_realname(client_fd, realname);
}
