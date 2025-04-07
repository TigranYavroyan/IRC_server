#pragma once

#include <vector>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>

typedef uint16_t port_id;

template <port_id PORT>
class IRC_server {
    std::vector<int> clients;
    int server_fd;
    int max_fd;
public:
    IRC_server () {}
    ~IRC_server ();
public:
    void close_connection_all ();
    void setupServer ();
    void run ();
};

template <port_id PORT>
void IRC_server<PORT>::run () {
    int client_fd = 0;

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("accept");
            _exit(1);
        }
        std::cout << "Someone connected..." << std::endl;

        clients.push_back(client_fd);
    }
}

#include "irc_server.tpp"