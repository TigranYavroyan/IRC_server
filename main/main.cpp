#include <IRCServer.hpp>
#include <InputValidator.hpp>
#include <InputError.hpp>
#include <ServerError.hpp>
#include <UserTable.hpp>
#include <User.hpp>

int main (int argc, char* argv[]) try {
    std::pair<int, std::string> input = InputValidator::validate_input(argc, argv);

    IRCServer server(input.first, input.second);
    server.setupServer();
    server.run();
}
catch (const IRC::InputError& ex) {
    std::clog << "InputError: " << ex.what() << std::endl;
}
catch (const IRC::ServerError& ex) {
    std::clog << "ServerError: " << ex.what() << std::endl;
}
catch (const IRC::exception& ex) {
    std::clog << "Excetion: " << ex.what() << std::endl;
}