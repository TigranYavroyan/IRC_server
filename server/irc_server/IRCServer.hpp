#ifndef IRCServer_HPP
#define IRCServer_HPP

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

#include <EventHandler.hpp>
#include <Helpers.hpp>
#include <Parsing.hpp>
#include <Constants.hpp>

class IRCServer {
	const int PORT;
    const std::string password;

    std::vector<int> clients;
    std::set<int> auths;
    
    int server_fd;
    EventHandler eventhandler;
public:
    IRCServer (int port, const std::string _password): PORT(port), password(_password) {}
    ~IRCServer ();
public:
    void closeConnectionAll ();
    void setupServer ();
    void run ();
private:
    void __acceptConnection ();
    void __messageChecking (int client);
    void __broadcastMessage (int client, const std::string& msg);
    bool __is_client_logged_in (int client) const;
};

#endif // IRCServer_HPP
