#include "UserCommand.hpp"
#include "User.hpp"
#include "IRCServer.hpp"
#include <cstring>
#include <Replies.hpp>

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

    // need to get the reference , not copy to affect the change of username
    UserTable& user_table = server.getUserTable();
    User user = user_table.get_user(client_fd);

    if (!user.is_user()) {
        std::string msg = Replies::err_alreadyRegistered("USER", user.get_nickname());
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }

    if (tokens.size() < 5 || tokens[4][0] != ':') {
        std::string msg = Replies::err_notEnoughParam("USER", user.get_nickname());
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }

    std::string username = tokens[1];
    std::string realname = __merge_form(tokens, 4);
    if (realname[0] == ':')
        realname = realname.substr(1);

    user_table.set_user_username(client_fd, username);
    user_table.set_user_realname(client_fd, realname);

    // if (user.get_is_auth() && user.get_has_nick()) {
    //     std::cout << ">> Registered client " << client_fd << " [" << user.get_nickname() << "]\n";
    // }
}
