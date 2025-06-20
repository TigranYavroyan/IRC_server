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
	
	std::string channel_name = tokens[1];

	if (!server.is_channel_exist(channel_name)) {
		msg = Replies::err_noSuchChannel(user.get_nickname(), channel_name);
		user.sendMessage(msg);
		return;
	}
	
	Channel& channel = server.getChannel(channel_name);

	if (tokens.size() == 2 || (tokens.size() > 2 && tokens[2][0] != '+' && tokens[2][0] != '-')) {
		user.sendMessage(Replies::channelModes(user.get_nickname(), channel_name, channel.getModes()));
        user.sendMessage(Replies::creationOnTime(user.get_nickname(), channel_name, channel.getTimestamp()));
		return;
	}

	if (!channel.getUserByNick(user.get_nickname())) {
        msg = Replies::err_noOnThatChannel(user.get_nickname(), channel_name);
		user.sendMessage(msg);
		return;
	}

	if (!channel.isOperator(&user)) {
		msg = Replies::err_notOperator(user.get_nickname(), channel.getName());
		user.sendMessage(msg);
		return;
	}

	Debugger::print_input(tokens);

	std::vector<ModeChange> raw = Helpers::parse_modes_raw(tokens);
	std::vector<ModeChange> operations = Helpers::filter_valid_modes(user, raw, channel);

	Debugger::mode_print_commands(operations);

	for (std::size_t i = 0; i < operations.size(); ++i) {
		ModeChange& cmd = operations[i];

		if (cmd.action == '+') {
			__mode_set(cmd, channel);
		}
		else if (cmd.action == '-') {
			__mode_remove(cmd, channel);
		}
		else {
			std::cerr << "Error: cmd.action must be + or - (" << cmd.action << ")\nCommand execution is stoped" << std::endl;
			break;
		}
	}

	if (operations.size() > 0)
		channel.broadcast(Replies::message(user, __format_mode_reply(channel.getName(), operations)));
}

void Mode::__mode_set(const ModeChange& cmd, Channel& channel) {
	char option = cmd.mode;
	User* new_operator;

	switch (option)
	{
		case 'i':
			channel.setInviteOnly(true);
			break;
		case 'o':
			new_operator = channel.getUserByNick(cmd.param);
			channel.addOperator(new_operator);
			break;
		case 'k':
			channel.setKey(cmd.param);
			break;
		case 't':
			channel.setTopicRestricted(true);
			break;
		case 'l':
			channel.setUserLimit(std::atoi(cmd.param.c_str()));
			break;
		default:
			std::cerr << "Invalid option is remained in ModeChange: (" << option << ')' << std::endl;
			break;
	}
}

void Mode::__mode_remove(const ModeChange& cmd, Channel& channel) {
	char option = cmd.mode;
	User* to_remove;

	switch (option)
	{
		case 'i':
			channel.setInviteOnly(false);
			break;
		case 'o':
			to_remove = channel.getUserByNick(cmd.param);
			channel.removeOperator(to_remove);
			break;
		case 'k':
			channel.removeKey();
			break;
		case 't':
			channel.setTopicRestricted(false);
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