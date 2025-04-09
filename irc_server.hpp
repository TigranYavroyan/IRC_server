#pragma once

#include <vector>
#include <iostream>
#include <set>
#include <sstream>
#include <algorithm>

#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <sys/select.h>

typedef uint16_t port_id;

template <port_id PORT>
class IRC_server {
    std::vector<int> clients;
    const std::string password;
    std::set<int> auths;
    
    const std::size_t BUFFER_SIZE;
    int server_fd;
    int max_fd;
    fd_set all_fdset;
    fd_set read_fdset;
public:
    IRC_server (const std::string& _password = "hello") : password(_password), BUFFER_SIZE(1000) {}
    ~IRC_server ();
public:
    void closeConnectionAll ();
    void setupServer ();
    void run ();
private:
    void __acceptConnection ();
    void __messageChecking (int client);
    void __broadcastMessage (int client, const std::string& msg);
    bool __is_client_logged_in (int client) const;
    static void __right_trim (std::string& str, const char* delims = " \n\t\r");
    static void __left_trim (std::string& str, const char* delims = " \n\t\r");
    static void __trim (std::string& str, const char* delims = " \n\t\r");

    template <typename T>
    static std::string to_string(const T& value);
};

template <port_id PORT>
bool IRC_server<PORT>::__is_client_logged_in (int client) const {
    return auths.find(client) != auths.end();
}

template <port_id PORT>
void IRC_server<PORT>::__broadcastMessage (int client, const std::string& msg) {
    for (std::size_t i = 0; i < clients.size(); ++i) {
        if (client != clients[i])
            send(clients[i], msg.c_str(), msg.size(), 0);
    }
}

template <port_id PORT>
void IRC_server<PORT>::__messageChecking (int client) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(client, buffer, BUFFER_SIZE, 0);
    
    if (bytes_received <= 0) {
        close(client);
        FD_CLR(client, &all_fdset);
        clients.erase(std::find(clients.begin(), clients.end(), client));
        auths.erase(client);
        std::cout << "Client disconnected: " << client << std::endl;
        return;
    }
    std::string message = buffer;
    __trim(message);
    if (!__is_client_logged_in(client)) {
        if (message == password)
        {
            const char* login_msg = "Welcome to the chat!\n";
            std::cout << "New client connected: " << client << std::endl;
            send(client, login_msg, strlen(login_msg), 0);
            auths.insert(client);
        }
        else {
            if (message.empty())
                return;

            const char* login_msg = "The password is wrong: ";
            send(client, login_msg, strlen(login_msg), 0);
        }
    }
    else {
        if (message.empty())
            return;

        std::string log = ("Client " + to_string(client) + ": " + message + '\n');
        std::cout << log;
        std::cout.flush();
    
        __broadcastMessage(client, log);
    }
}

template <port_id PORT>
void IRC_server<PORT>::run () {
    while (true) {
        read_fdset = all_fdset;

        if (select(max_fd + 1, &read_fdset, NULL, NULL, NULL) < 0)
            perror("select");

        if (FD_ISSET(server_fd, &read_fdset))
            __acceptConnection();

        for (std::size_t i = 0; i < clients.size(); ++i) {
            if (FD_ISSET(clients[i], &read_fdset))
                __messageChecking(clients[i]);
        }
    }
}

#include "irc_server.tpp"