#include <Logger.hpp>
#include <iostream>

void Logger::__info () {
    std::cout << "[INFO] ";
}

void Logger::__info_start () {
    std::cout << " ---------- [INFO START] ----------" << std::endl;
}

void Logger::__info_end () {
    std::cout << " ---------- [INFO END] ----------" << std::endl;
}


void Logger::client_join (const std::string& nickname, const std::string& channelname) {
    __info();
    std::cout << "User " << nickname << " added in " << channelname << std::endl;
}

void Logger::client_try_to_connect (int socket_fd) {
    __info();
    std::cout << "Client " << socket_fd << " is trying to register" << std::endl;
}

void Logger::client_registered (int socket_fd) {
    __info();
    std::cout << "Client " << socket_fd << " is registered" << std::endl;
}

void Logger::client_disconnected (int socket_fd) {
    __info();
    std::cout << "Client " << socket_fd << " disconnected" << std::endl;
}

void Logger::server_on (short port, const std::string& password) {
    __info();
    std::cout << "Listening on " << 0 << ":" << port << "..." << std::endl;
    __info();
    std::cout << "The password: " << password << std::endl;
}

