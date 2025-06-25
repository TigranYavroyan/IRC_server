#include "Nick.hpp"
#include "IRCServer.hpp"
#include "User.hpp"

Nick::Nick(IRCServer& server) : ACommand(server) {}

Nick::~Nick() {}

void Nick::execute(int client_socket, const std::vector<std::string>& tokens) {
    UserTable& user_table = server.getUserTable();
    User client = user_table.get_user(client_socket);
    std::string msg;
    std::string old_nick = client.get_nickname();

    if (tokens.size() < 2) {
        msg = Replies::err_noNickName("NICK", client.get_nickname());
        client.sendMessage(msg);
        return;
    }

    std::string new_nick = tokens[1];

    if (!InputValidator::is_valid_nickname(new_nick)) {
        msg = Replies::err_oneUsNick(old_nick, new_nick);
        client.sendMessage(msg);
        return;
    }

    if (user_table.is_nickname_taken(new_nick) || new_nick == "bot") {
        msg = Replies::err_nickInUse("NICK", new_nick);
        client.sendMessage(msg);
        return;
    }

    user_table.set_user_nickname(client_socket, new_nick);
}

