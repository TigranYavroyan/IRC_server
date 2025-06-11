#include "Nick.hpp"
#include "IRCServer.hpp"
#include "User.hpp"

Nick::Nick(IRCServer& server) : ACommand(server) {}

Nick::~Nick() {}

void Nick::execute(int client_socket, const std::vector<std::string>& tokens) {
    UserTable& user_table = server.getUserTable();
    User client = user_table.get_user(client_socket);

    if (tokens.size() < 2) 
    {
        std::string msg = Replies::err_noNickName("NICK", client.get_nickname());
        client.sendMessage(msg);
        return;
    }

    std::string new_nick = tokens[1];

    if (user_table.is_nickname_taken(new_nick))
    {
        std::string msg = Replies::err_nickInUse("NICK", new_nick);
        client.sendMessage(msg);
        return;
    }

    std::string old_nick = client.get_nickname();
    user_table.set_user_nickname(client_socket, new_nick);

    // std::string msg = Replies::nickChange(old_nick, new_nick);
    // client.sendMessage(msg);
}

