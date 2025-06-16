#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <vector>

class Logger {
    static void __info ();
    static void __info_start ();
    static void __info_end ();
public:
    static void client_join (const std::string& nickname, const std::string& channelname);
    static void client_try_to_connect (int socket_fd);
    static void client_registered (int socket_fd);
    static void client_disconnected (int socket_fd);
    static void server_on (short port, const std::string& password);
};

#endif // LOGGER_HPP