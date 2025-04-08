#pragma once

#include <vector>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <sys/select.h>

typedef uint16_t port_id;

template <port_id PORT>
class IRC_server {
    std::vector<int> clients;
    int server_fd;
    int max_fd;
    fd_set all_fdset;
    fd_set read_fdset;
    const std::size_t BUFFER_SIZE;

public:
    IRC_server () : BUFFER_SIZE(1000) {}
    ~IRC_server ();
public:
    void closeConnectionAll ();
    void setupServer ();
    void run ();
private:
    void acceptConnection ();
    void messageChecking (int client);
    void broadcastMessage (int client, const std::string& msg);
};

template <port_id PORT>
void IRC_server<PORT>::broadcastMessage (int client, const std::string& msg) {
    for (int to_send : clients) {
        if (client != to_send)
            send(to_send, msg.c_str(), msg.size(), 0);
    }
}

template <port_id PORT>
void IRC_server<PORT>::messageChecking (int client) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(client, buffer, BUFFER_SIZE, 0);

    if (bytes_received <= 0) {
        close(client);
        FD_CLR(client, &all_fdset);
        clients.erase(std::find(clients.begin(), clients.end(), client));
        std::cout << "Client disconnected: " << client << std::endl;
        return;
    }

    std::string message = "Client " + std::to_string(client) + ": " + buffer;
    std::cout << message;

    broadcastMessage(client, message);
}

template <port_id PORT>
void IRC_server<PORT>::run () {
    while (true) {
        read_fdset = all_fdset;

        if (select(max_fd + 1, &read_fdset, NULL, NULL, NULL) < 0)
            perror("select");

        if (FD_ISSET(server_fd, &read_fdset))
            acceptConnection();

        for (int client : clients) {
            if (FD_ISSET(client, &read_fdset))
                messageChecking(client);
        }
    }
}

#include "irc_server.tpp"