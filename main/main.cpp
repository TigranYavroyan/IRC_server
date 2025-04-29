#include <IRCServer.hpp>
#include <Parsing.hpp>
#include <InputError.hpp>

int main (int argc, char* argv[]) try {
    std::pair<int, std::string> input = Parsing::validate_input(argc, argv);

    // IRCServer server(input.first, input.second);
    // server.setupServer();
    // server.run();
}
catch (const IRC::InputError& ex) {
    std::clog << "InputError: " << ex.what() << std::endl;
}
catch (const IRC::exception& ex) {
    std::clog << "Error: " << ex.what() << std::endl;
}