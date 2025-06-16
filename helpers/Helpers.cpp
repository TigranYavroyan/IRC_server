#include <Helpers.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <Logger.hpp>

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

std::string Helpers::reverseDNS(struct sockaddr_in &clientAddr)
{

	const char *ip = inet_ntoa(clientAddr.sin_addr);
	struct addrinfo hints, *res = NULL;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(ip, NULL, &hints, &res) == 0 && res->ai_canonname)
	{
		std::string hn(res->ai_canonname);
		freeaddrinfo(res);
		return hn;
	}
	if (res)
		freeaddrinfo(res);
	return std::string(ip);
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

std::vector<ModeChange> Helpers::mode_parse_command(const std::vector<std::string>& raw_input, std::string& err_msg) {

    std::vector<ModeChange> result;
    std::vector<std::string> args = __normalize_mode_arguments(raw_input);

    const std::string& command = args[0];
    const std::string& channel = args[1];

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

    for (std::size_t i = 0; i < modeBlocks.size(); ++i) {
        const std::string& modeStr = modeBlocks[i];
        if (modeStr.empty()) continue;

        char currentAction = 0;
        for (std::size_t j = 0; j < modeStr.size(); ++j) {
            char c = modeStr[j];
            if (c == '+' || c == '-') {
                currentAction = c;
            } else {
                if (currentAction == 0) {
                    std::cerr << "Error: Mode character '" << c << "' has no associated + or - action.\n";
                    continue;
                }

                if (!std::isalpha(c) || !__is_valid_mode_char(c)) {
                    std::cerr << "Error: Invalid or unsupported mode character '" << c << "'\n";
                    continue;
                }

                ModeChange mc;
                mc.action = currentAction;
                mc.mode = c;

                if (__mode_needs_param(c, currentAction)) {
                    if (paramIndex < params.size()) {
                        mc.param = params[paramIndex++];
                    } else {
                        std::cerr << "Error: Mode '" << c << "' requires parameter but none provided.\n";
                        continue;
                    }
                }

                result.push_back(mc);
            }
        }
    }

    if (paramIndex < params.size()) {
        std::cerr << "Warning: " << (params.size() - paramIndex)
                  << " unused parameter(s): ";
        for (std::size_t i = paramIndex; i < params.size(); ++i)
            std::cerr << params[i] << " ";
        std::cerr << std::endl;
    }

    return result;
}
