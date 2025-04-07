#include "irc_server.hpp"

int main() {
    IRC_server<8080> server;
    server.setupServer();
    server.run();
}
