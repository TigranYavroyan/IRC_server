#include "UserCommand.hpp"
#include "User.hpp"
#include "IRCServer.hpp"
#include <cstring>

UserCommand::UserCommand(IRCServer& server) : ACommand(server) {}

UserCommand::~UserCommand() {}

std::string UserCommand::__merge_form(const std::vector<std::string>& tokens, size_t start){
	std::string result;
	for (size_t i = start; i < tokens.size(); ++i) {
		if (i != start)
			result += " ";
		result += tokens[i];
	}
	if (!result.empty() && result[0] == ':')
		result.erase(0, 1);
	return result;
}

void UserCommand::execute(int client_fd, const std::vector<std::string>& tokens) {
    User user = server.getUserTable().get_user(client_fd);

    if (user.get_has_user_info()) {
        const char* msg = "ERROR: You have already registered.\r\n";
        send(client_fd, msg, strlen(msg), 0);
        return;
    }

    if (tokens.size() < 5 || tokens[4][0] != ':') {
        const char* msg = "ERROR: Invalid USER command format. Usage: USER <username> * * :<realname>\r\n";
        send(client_fd, msg, strlen(msg), 0);
        return;
    }

    std::string username = tokens[1];
    std::string realname = __merge_form(tokens, 4);
    if (realname[0] == ':')
        realname = realname.substr(1);

    user.set_username(username);
    user.set_realname(realname);
    user.set_has_user_info(true);

    std::string ok = "USER accepted: " + username + " (" + realname + ")\r\n";
    send(client_fd, ok.c_str(), ok.size(), 0);

    if (user.get_is_auth() && user.get_has_nick()) {
        const char* welcome = "Welcome to the IRC server!\r\n";
        send(client_fd, welcome, strlen(welcome), 0);
        std::cout << ">> Registered client " << client_fd << " [" << user.get_nickname() << "]\n";
    }
}
