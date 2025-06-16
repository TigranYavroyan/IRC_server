#include <Mode.hpp>
#include <IRCServer.hpp>
#include <Debugger.hpp>

void Mode::execute (int socket_fd, const std::vector<std::string>& tokens) {
	UserTable& usertable = server.getUserTable();
	User& user = usertable[socket_fd];
	std::string msg;

	if (tokens.size() == 1) {
		msg = Replies::err_notEnoughParam(tokens[0], user.get_nickname());
		user.sendMessage(msg);
		return;
	}

	if (tokens.size() == 2) {
		std::string channel_name = tokens[1];
		if (!server.is_channel_exist(channel_name)) {
			msg = Replies::err_noSuchChannel(user.get_nickname(), channel_name);
			user.sendMessage(msg);
		}
		else {
			/**
			 * $ Must send the channel mode with 324 reply number
			 * 
			 * $ <server_name> 324 <nickname> <channel_name> +nt
			*/
			// msg = Replies::channelModes();
			// user.sendMessage(msg);
		}
		return;
	}

	Debugger::print_input(tokens);
	std::vector<ModeChange> operations = Helpers::mode_parse_command(user, tokens);
	Debugger::mode_print_commands(operations);

	/**
	 * struct ModeChange {
	 *		char action;
	 *		char mode;
	 *		std::string param;
	 *	};
	*/

	/**
	 * i (invite)
	 * t (topic)
	 * o (operator) <nickname>
	 * k (key or password) + (param), -
	 * l (user limit) + (param), -
	*/
	Channel& channel = server.getChannel(tokens[1]);

	if (!channel.isOperator(&user)) {
		msg = Replies::err_notOperator(channel.getName());
		user.sendMessage(msg);
		return;
	}

	for (std::size_t i = 0; i < operations.size(); ++i) {
		ModeChange& cmd = operations[i];

		if (cmd.action == '+') {
			__mode_set(cmd, user, channel);
		}
		else if (cmd.action == '-') {
			__mode_remove(cmd, user, channel);
		}
		else {
			std::cerr << "Error: cmd.action must be + or - (" << cmd.action << ")\nCommand execution is stoped" << std::endl;
			break;
		}
	}

	channel.broadcast(Replies::message(user, __format_mode_reply(channel.getName(), operations)));
}

void Mode::__mode_set(const ModeChange& cmd, User& user, Channel& channel) {
	char option = cmd.mode;
	User* new_operator = channel.getUserByNick(cmd.param);
	int limit = std::atoi(cmd.param.c_str());

	switch (option)
	{
		case 'i':
			channel.setInviteOnly(true);
			break;
		case 'o':
			if (!new_operator) {
				/**
				 * 
				 * $ some logic
				 * 
				*/
			}
			channel.addOperator(new_operator);
			break;
		case 'k':
			channel.setKey(cmd.param);
			break;
		case 't':
			channel.setTopicRestricted(false);
			break;
		case 'l':
			if (limit == 0) {
				/**
				 * 
				 * $ some logic
				 * 
				*/
				break;
			}
			channel.setUserLimit(limit);
			break;
		default:
			std::cerr << "Invalid option is remained in ModeChange: (" << option << ')' << std::endl;
			break;
	}
}

void Mode::__mode_remove(const ModeChange& cmd, User& user, Channel& channel) {
	char option = cmd.mode;
	User* to_remove = channel.getUserByNick(cmd.param);

	switch (option)
	{
		case 'i':
			channel.setInviteOnly(false);
			break;
		case 'o':
			if (!to_remove) {
				/**
				 * 
				 * $ some logic
				 * 
				*/
			}
			channel.removeOperator(to_remove);
			break;
		case 'k':
			channel.removeKey();
			break;
		case 't':
			channel.setTopicRestricted(true);
			break;
		case 'l':
			channel.setUserLimit(0);
			break;
		default:
			std::cerr << "Invalid option is remained in ModeChange: (" << option << ')' << std::endl;
			break;
	}
}

std::string Mode::__format_mode_reply(const std::string& channel, const std::vector<ModeChange>& operations) {
    std::ostringstream reply;
    std::ostringstream modeString;
    std::vector<std::string> params;

    // Start with server prefix
    reply << "MODE " << channel << " ";

    char currentAction = '\0';
    for (std::size_t i = 0; i < operations.size(); ++i) {
        const ModeChange& mc = operations[i];
        if (mc.action != currentAction) {
            currentAction = mc.action;
            modeString << currentAction;
        }
        modeString << mc.mode;
        if (!mc.param.empty()) {
            params.push_back(mc.param);
        }
    }

    reply << modeString.str();

    for (std::size_t i = 0; i < params.size(); ++i) {
        reply << " " << params[i];
    }

    return reply.str();
}