#include "Nick.hpp"
#include "IRCServer.hpp"
#include "User.hpp"

Nick::Nick(IRCServer& server) : ACommand(server) {}

Nick::~Nick() {}

void Nick::execute(int client_socket, const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) 
	{
        std::string msg = "ERROR: Nickname required.\r\n";
        send(client_socket, msg.c_str(), msg.length(), 0);
        return;
    }

    std::string new_nick = tokens[1];

    UserTable& user_table = server.getUserTable();
    if (user_table.is_nickname_taken(new_nick))
	{
        std::string msg = "ERROR: Nickname already in use.\r\n";
        send(client_socket, msg.c_str(), msg.length(), 0);
        return;
    }

    user_table.set_user_nickname(client_socket, new_nick);
    std::string msg = "Your nickname is now: " + new_nick + "\r\n";
    send(client_socket, msg.c_str(), msg.length(), 0);
}
