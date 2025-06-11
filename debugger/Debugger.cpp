#include <Debugger.hpp>

void Debugger::print_tokens (const std::vector<std::string>& tokens) {
    __debug_start();
    if (!tokens.empty()) {
		std::cout << "Command: " << tokens[0] << std::endl;
		for (int i = 1; i < tokens.size(); ++i) {
			std::cout << "arg[" << i << "]: " << tokens[i] << std::endl;
		}
	}
    else {
        std::cout << "empty tokens" << std::endl;
    }
    __debug_end();
}

void Debugger::__debug_start () {
    std::cout << " ---------- [DEBUG START] ----------" << std::endl;
}

void Debugger::__debug_end () {
    std::cout << " ---------- [DEBUG END] ----------" << std::endl;
}

std::string Debugger::__debug_msg () {
    return std::string("[DEBUG]: ");
}

void Debugger::client_pass (const User& client) {
    std::cout << __debug_msg() << "Client " << client.get_socket_fd() << " inputed the pass" << std::endl;
}

void Debugger::client_nick (const User& client) {
    std::cout << __debug_msg() << "Client " << client.get_socket_fd() << " got nickname" << std::endl;
}

void Debugger::client_user (const User& client) {
    std::cout << __debug_msg() << "Client " << client.get_socket_fd() << " got username" << std::endl;
}