#include <irc_server.hpp>

int main() {
    IRC_server server;
    server.setupServer();
    server.run();
}
