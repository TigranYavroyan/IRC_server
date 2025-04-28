#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

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

class IRC_server {
    std::vector<int> clients;
    const std::string password;
	const int PORT;
    std::set<int> auths;
    
    const std::size_t BUFFER_SIZE;
    int server_fd;
    EventHandler eventhandler;
public:
    IRC_server (const std::string& _password = "hello", int port = 8080) : password(_password), PORT(port), BUFFER_SIZE(1024) {}
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

#endif // IRC_SERVER_HPP
