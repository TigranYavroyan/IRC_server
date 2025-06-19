#include <Helpers.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <Logger.hpp>
#include <Replies.hpp>
#include <IRCServer.hpp>
#include <cstdlib>

void Helpers::right_trim (std::string& str, const char* delims) {
    str.erase(str.find_last_not_of(delims) + 1);
}

void Helpers::left_trim (std::string& str, const char* delims) {
    str.erase(0, str.find_first_not_of(delims));
}

void Helpers::trim (std::string& str, const char* delims) {
    left_trim(str, delims);
    right_trim(str, delims);
}


std::vector<std::string> Helpers::parse_msg (const char* msg) {
	std::vector<std::string> tokens;
	std::string input(msg);
	std::string token;
	std::istringstream ss(input);

	while (ss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

std::vector<std::string> Helpers::parse_msg (const std::string& msg) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream ss(msg);

	while (ss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

std::string Helpers::merge_from(const std::vector<std::string>& tokens, size_t start) {
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

std::vector<std::string> Helpers::split_by_delim(const std::string& input, char delim) {
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type delimPos;

    while ((delimPos = input.find(delim, start)) != std::string::npos) {
        result.push_back(input.substr(start, delimPos - start));
        start = delimPos + 1;
    }

    result.push_back(input.substr(start));

    return result;
}


bool Helpers::__is_valid_mode_char(char c) {
    return c == 'i' || c == 't' || c == 'k' || c == 'l' || c == 'o';
}

bool Helpers::__mode_needs_param(char mode, char action) {
    if (mode == 'o' || mode == 'k') return true;
    if (mode == 'l') return action == '+';
    return false;
}

std::vector<std::string> Helpers::__normalize_mode_arguments(const std::vector<std::string>& args) {
    std::vector<std::string> result;

    result.push_back(args[0]);
    result.push_back(args[1]);

    std::string currentModeBlock;


    for (std::size_t i = 2; i < args.size(); ++i) {
        const std::string& token = args[i];

        if (token == "+" || token == "-") {
            if (!currentModeBlock.empty())
                result.push_back(currentModeBlock);
            currentModeBlock = token;
        }
        else if (!currentModeBlock.empty() && token.size() == 1 && std::isalpha(token[0])) {
            currentModeBlock += token;
        }
        else {
            if (!currentModeBlock.empty()) {
                result.push_back(currentModeBlock);
                currentModeBlock.clear();
            }
            result.push_back(token);
        }
    }

    if (!currentModeBlock.empty())
        result.push_back(currentModeBlock);

    return result;
}

int __number_validation (const std::string& input, bool& valid) {
	int limit = 0;

	for (std::size_t i = 0; i < input.size(); ++i) {
		if (!std::isdigit(input[i])) {
			valid = false;
			break;
		}
	}

	if (valid) {
		limit = std::atoi(input.c_str());
		if (limit <= 0) {
			valid = false;
		}
	}

	return limit;
}

std::vector<ModeChange> Helpers::parse_modes_raw(const std::vector<std::string>& raw_input) {
    std::vector<ModeChange> result;
    std::vector<std::string> args = __normalize_mode_arguments(raw_input);

    if (args.size() < 2)
        return result;

    std::vector<std::string> modeBlocks;
    std::vector<std::string> params;

    for (std::size_t i = 2; i < args.size(); ++i) {
        const std::string& arg = args[i];
        if (!arg.empty() && (arg[0] == '+' || arg[0] == '-')) {
            modeBlocks.push_back(arg);
        } else {
            params.push_back(arg);
        }
    }

    std::size_t paramIndex = 0;
    char currentAction = 0;

    for (std::size_t i = 0; i < modeBlocks.size(); ++i) {
        const std::string& modeStr = modeBlocks[i];
        if (modeStr.empty()) continue;

        for (std::size_t j = 0; j < modeStr.size(); ++j) {
            char c = modeStr[j];
            if (c == '+' || c == '-') {
                currentAction = c;
            } else {
                if (!std::isalpha(c)) continue; // skip garbage

                ModeChange mc;
                mc.action = currentAction;
                mc.mode = c;

                if (__mode_needs_param(c, currentAction) && paramIndex < params.size()) {
                    mc.param = params[paramIndex++];
                }

                result.push_back(mc);
            }
        }
    }

    return result;
}

std::vector<ModeChange> Helpers::filter_valid_modes(const User& user, const std::vector<ModeChange>& input, Channel& channel) {
    std::vector<ModeChange> valid;
    std::string err_msg;

    for (std::size_t i = 0; i < input.size(); ++i) {
        const ModeChange& mc = input[i];

        if (!__is_valid_mode_char(mc.mode)) {
            err_msg = Replies::err_invaliDModeParm(user.get_nickname(), mc.mode);
            user.sendMessage(err_msg);
            continue;
        }

        if (mc.mode == 'l' && mc.action == '+') {
            if (mc.param.empty()) {
                err_msg = Replies::err_notEnoughParam("MODE", user.get_nickname());
                user.sendMessage(err_msg);
                continue;
            }

            bool valid_number = true;
            int limit = __number_validation(mc.param.c_str(), valid_number);
            if (!valid_number || limit <= 0) {
                err_msg = Replies::err_notEnoughParam("MODE", user.get_nickname());
                user.sendMessage(err_msg);
                continue;
            }
        }

        if (mc.mode == 'o') {
            if (mc.param.empty()) {
                err_msg = Replies::err_notEnoughParam("MODE", user.get_nickname());
                user.sendMessage(err_msg);
                continue;
            }

            if (!channel.getUserByNick(mc.param)) {
                err_msg = Replies::err_recipientNotInChannel(user.get_nickname(), mc.param, channel.getName());
                user.sendMessage(err_msg);
                continue;
            }
            
        }

        valid.push_back(mc);
    }

    return valid;
}
