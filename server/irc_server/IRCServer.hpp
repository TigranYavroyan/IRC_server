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
#include <InputValidator.hpp>
#include <Constants.hpp>
#include <Executor.hpp>
#include <Channel.hpp>
#include <Replies.hpp>

class IRCServer {
	const int PORT;
    const std::string password;
    int server_fd;

    std::map<std::string, Channel> channels;
    std::map<int, std::string> user_msg_buffer;
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
    const std::string& getPassword() const;
    UserTable& getUserTable();
    Channel& getChannel(const std::string& channel_name);
    void removeFromChannel (User& user, const std::string& channel_name);
    void removeChannel (const std::string& channel_name);
    bool is_channel_exist (const std::string& channel_name);
private:
    void __message_execution (int client, std::string& message);
    void __user_disconnect (int client);
    void __accept_connection ();
    void __message_checking (int client);
};

#endif // IRCSERVER_HPP