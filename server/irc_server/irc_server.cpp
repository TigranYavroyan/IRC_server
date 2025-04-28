#include "irc_server.hpp"
#include <errno.h>

IRC_server::~IRC_server () {
    closeConnectionAll();
}

void IRC_server::closeConnectionAll () {
    if (server_fd >= 0)
        close(server_fd);
    for (std::size_t i = 0; i < clients.size(); ++i) {
        close(clients[i]);
    }
}

void IRC_server::setupServer () {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        _exit(1);
    }

    int opt = 1;
    fcntl(server_fd, F_SETFL, O_NONBLOCK);    
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        _exit(1);
    }

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // the IP
    server_addr.sin_port = htons(PORT); // the PORT

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        perror("bind");
        _exit(1);
    }

    if (listen(server_fd, SOMAXCONN)) {
        perror("listen");
        _exit(1);
    }

    eventhandler.subscribe_read(server_fd);

    std::cout << "Listening on " << INADDR_ANY << ":" << PORT << "..." << std::endl;
}


void IRC_server::__acceptConnection () {
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    std::memset(&client_addr, 0, addr_len);

    int new_client = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    fcntl(new_client, F_SETFL, O_NONBLOCK);
   
    if (new_client < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("accept");
        }
        return;
    }

    clients.push_back(new_client);
    eventhandler.subscribe_read(new_client);

    std::cout << "New client is trying to connect: " << new_client << "\n";
    std::string welcome_msg = "Welcome to the IRC-like chat server!\nEnter the password: ";
    if (send(new_client, welcome_msg.c_str(), welcome_msg.size(), 0) < 0)
        perror("send");
}

void IRC_server::__right_trim (std::string& str, const char* delims) {
    str.erase(str.find_last_not_of(delims) + 1);
}

void IRC_server::__left_trim (std::string& str, const char* delims) {
    str.erase(0, str.find_first_not_of(delims));
}

void IRC_server::__trim (std::string& str, const char* delims) {
    __left_trim(str, delims);
    __right_trim(str, delims);
}

template <typename T>
std::string IRC_server::to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}


bool IRC_server::__is_client_logged_in (int client) const {
    return auths.find(client) != auths.end();
}

void IRC_server::__broadcastMessage (int client, const std::string& msg) {
    for (std::size_t i = 0; i < clients.size(); ++i) {
        if (client != clients[i])
            send(clients[i], msg.c_str(), msg.size(), 0);
    }
}

void IRC_server::__messageChecking (int client) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(client, buffer, BUFFER_SIZE, 0);
    
    if (bytes_received <= 0) {
        if (bytes_received == 0 || (errno != EAGAIN && errno != EWOULDBLOCK)) {
            close(client);
            eventhandler.unsubscribe_read(client);
            clients.erase(std::find(clients.begin(), clients.end(), client));
            auths.erase(client);
            std::cout << "Client disconnected: " << client << std::endl;

        }
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

void IRC_server::run () {
    while (true) {
        if (eventhandler.wait_event() < 0)
            perror("select");

        if (eventhandler.is_read_event(server_fd))
            __acceptConnection();

        for (std::size_t i = 0; i < clients.size(); ++i) {
            if (eventhandler.is_read_event(clients[i]))
                __messageChecking(clients[i]);
        }
    }
}