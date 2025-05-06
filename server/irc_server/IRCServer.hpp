#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

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
#include <fcntl.h>

#include <exception.hpp>
#include <EventHandler.hpp>
#include <Helpers.hpp>
#include <Parsing.hpp>
#include <Constants.hpp>
#include <Executor.hpp>

class IRCServer {
	const int PORT;
    const std::string password;
    int server_fd;

    UserTable user_table;
    EventHandler eventhandler;
    Executor executor;
public:
    IRCServer (int port, const std::string& _password);
    ~IRCServer ();
public:
    void closeConnectionAll ();
    void setupServer ();
    void run ();
private:
    void __acceptConnection ();
    void __messageChecking (int client);
    void __broadcastMessage (int client, const std::string& msg);
};

#endif // IRCSERVER_HPP