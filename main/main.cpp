#include <IRCServer.hpp>
#include <InputValidator.hpp>
#include <InputError.hpp>
#include <ServerError.hpp>
#include <UserTable.hpp>
#include <User.hpp>

#ifdef DEBUG
    #include <signal.h>

    bool stopServer = false;

    void signalHandle (int signal) {
        switch (signal)
        {
            case SIGINT:
                stopServer = true;
                break;
            default:
                break;
        }
    }
#endif

int main (int argc, char* argv[]) try {
    std::pair<int, std::string> input = InputValidator::validate_input(argc, argv);
    IRCServer server(input.first, input.second);

    #ifdef DEBUG
        signal(SIGINT, signalHandle);
    #endif

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
    std::clog << "Exception: " << ex.what() << std::endl;
}