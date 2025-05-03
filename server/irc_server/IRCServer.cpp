#include <IRCServer.hpp>
#include <errno.h>

IRCServer::IRCServer (int port, const std::string& _password): PORT(port), password(_password) {
    executor.set_server(*this);
}

IRCServer::~IRCServer () {
    closeConnectionAll();
}

void IRCServer::closeConnectionAll () {
    if (server_fd >= 0)
        close(server_fd);
    UserBySocketIter it = user_table.tsbegin();
    while (it != user_table.tsend()) {
        close(it->first);
        ++it;
    }
}

void IRCServer::setupServer () {
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

    eventhandler.subscribe_get(server_fd);

    std::cout << "Listening on " << INADDR_ANY << ":" << PORT << "..." << std::endl;
    std::cout << "The password: " << password << std::endl;
}

void IRCServer::run () {
    UserBySocketIter it;
    int socket_fd;

    while (true) {
        if (eventhandler.wait_event() < 0)
            perror("select");

        if (eventhandler.is_get_event(server_fd))
            __acceptConnection();

        it = user_table.tsbegin();
        while (it != user_table.tsend()) {
            // for escaping the iterator erasing case
            socket_fd = it->first;
            ++it;
            if (eventhandler.is_get_event(socket_fd))
                __messageChecking(socket_fd);
        }
    }
}

// ----------------------------- Private methods ---------------------------------

void IRCServer::__acceptConnection () {
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    std::memset(&client_addr, 0, addr_len);

    int new_client = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    
    if (new_client < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("accept");
        }
        return;
    }
    fcntl(new_client, F_SETFL, O_NONBLOCK);
    
    user_table.set_user(new_client);
    eventhandler.subscribe_get(new_client);

    std::cout << "New client is trying to connect: " << new_client << "\n";
    std::string welcome_msg = "Welcome to the IRC-like chat server!\nEnter the password: ";
    if (send(new_client, welcome_msg.c_str(), welcome_msg.size(), 0) < 0)
        perror("send");
}

// This function must be in channel
void IRCServer::__broadcastMessage (int client, const std::string& msg) {
    UserBySocketIter it = user_table.tsbegin();

    while (it != user_table.tsend()) {
        User to_send = user_table.get_user(it->first);
        ++it;
        if (client != to_send.get_socket_fd() && to_send.get_is_auth())
            send(to_send.get_socket_fd(), msg.c_str(), msg.size(), 0);
    }
}

void IRCServer::__messageChecking (int client) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(client, buffer, BUFFER_SIZE, 0);

    if (bytes_received <= 0) {
        if (bytes_received == 0 || (errno != EAGAIN && errno != EWOULDBLOCK)) {
            close(client);
            eventhandler.unsubscribe_get(client);
            user_table.remove_user(client);
            std::cout << "Client disconnected: " << client << std::endl;
        }
        return;
    }

    std::string message = buffer;
    Helpers::trim(message);

    std::vector<std::string> tokens = Parsing::parse_msg(message);

    executor.execute(client, tokens);

    // This section must moved into executors commands
    // ----
    if (!(user_table.get_user(client).get_is_auth())) {
        if (message == password)
        {
            const char* login_msg = "Welcome to the chat!\n";
            std::cout << "New client connected: " << client << std::endl;
            send(client, login_msg, strlen(login_msg), 0);
            user_table.set_user_auth(client);
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

        std::string log = ("Client " + Helpers::to_string(client) + ": " + message + '\n');
        std::cout << log;
        std::cout.flush();
    
        __broadcastMessage(client, log);
    }
    // ----
}