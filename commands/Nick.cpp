#include "Nick.hpp"
#include "IRCServer.hpp"
#include "User.hpp"

Nick::Nick(IRCServer& server) : ACommand(server) {}

Nick::~Nick() {}

void Nick::execute(int client_socket, const std::vector<std::string>& tokens) {
    UserTable& user_table = server.getUserTable();
    User user = user_table.get_user(client_socket);

    if (tokens.size() < 2) 
    {
        std::string msg = Replies::err_noNickName(user.get_nickname());
        send(client_socket, msg.c_str(), msg.length(), 0);
        return;
    }

    std::string new_nick = tokens[1];

    if (user_table.is_nickname_taken(new_nick))
    {
        std::string msg = Replies::err_nickInUse(new_nick);
        send(client_socket, msg.c_str(), msg.length(), 0);
        return;
    }

    std::string old_nick = user.get_nickname();
    user_table.set_user_nickname(client_socket, new_nick);

    std::string msg = Replies::nickChange(old_nick, new_nick);
    send(client_socket, msg.c_str(), msg.length(), 0);
}

