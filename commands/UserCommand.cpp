#include "UserCommand.hpp"
#include "User.hpp"
#include "InputValidator.hpp"
#include <cstring>
#include <IRCServer.hpp>

UserCommand::UserCommand(IRCServer& server) : ACommand(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(int client_fd, const std::vector<std::string>& tokens)
{
    // User user = server.getUserTable().get_user(client_fd);

    // if (user.get_has_user_info()) {
    //     const char* msg = "ERROR: You have already registered.\r\n";
    //     send(client_fd, msg, strlen(msg), 0);
    //     return;
    // }

    // if (tokens.size() < 5 || tokens[4][0] != ':') {
    //     const char* msg = "ERROR: Invalid USER command format. Usage: USER <username> * * :<realname>\r\n";
    //     send(client_fd, msg, strlen(msg), 0);
    //     return;
    // }

    // std::string username = tokens[1];
    // std::string realname = InputValidator::merge_from(tokens, 4);
    // if (realname[0] == ':')
    //     realname = realname.substr(1);

    // user.set_username(username);
    // // user.set_realname(realname);
    // user.set_is_auth(true);

    // std::string ok = "USER accepted: " + username + " (" + realname + ")\r\n";
    // send(client_fd, ok.c_str(), ok.size(), 0);

    // // Registration check
    // if (user.get_is_auth() && !user.get_nickname().empty()) {
    //     const char* welcome = "Welcome to the IRC server!\r\n";
    //     send(client_fd, welcome, strlen(welcome), 0);
    //     std::cout << ">> Registered client " << client_fd << " [" << user.get_nickname() << "]\n";
    // }
}
