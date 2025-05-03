#include <PrivMsg.hpp>

void PrivMsg::execute (int socket_fd, const std::vector<std::string>& tokens) {
	std::cout << "Privmsg called" << std::endl;
}
