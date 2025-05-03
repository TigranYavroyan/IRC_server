#include <Quit.hpp>

void Quit::execute (int socket_fd, const std::vector<std::string>& tokens) {
	std::cout << "Quit called" << std::endl;
}
