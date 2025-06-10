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
    if (server_fd < 0)
        throw IRC::exception(std::strerror(errno));

    int opt = 1;
    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        throw IRC::exception(std::strerror(errno));

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // the IP
    server_addr.sin_port = htons(PORT); // the PORT

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
        throw IRC::exception(std::strerror(errno));

    if (listen(server_fd, SOMAXCONN))
        throw IRC::exception(std::strerror(errno));

    eventhandler.subscribe_get(server_fd);

    std::cout << "Listening on " << INADDR_ANY << ":" << PORT << "..." << std::endl;
    std::cout << "The password: " << password << std::endl;
}

void IRCServer::run () {
    UserBySocketIter it;
    int socket_fd;

    while (true) {
        if (eventhandler.wait_event() < 0)
            throw IRC::exception(std::strerror(errno));


        if (eventhandler.is_get_event(server_fd))
            __accept_connection();

        it = user_table.tsbegin();
        while (it != user_table.tsend()) {
            // for escaping the iterator erasing case
            socket_fd = it->first;
            ++it;
            if (eventhandler.is_get_event(socket_fd))
                __message_checking(socket_fd);
        }
    }
}

// ----------------------------- Private methods ---------------------------------

void IRCServer::__accept_connection () {
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    std::memset(&client_addr, 0, addr_len);

    int new_client = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    
    if (new_client < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            throw IRC::exception(std::strerror(errno));
        return;
    }
    
    fcntl(new_client, F_SETFL, O_NONBLOCK);
    std::string hostname = Helpers::reverseDNS(client_addr);

    user_table.set_user(new_client, hostname);
    user_msg_buffer[new_client] = "";
    eventhandler.subscribe_get(new_client);

    std::cout << "Client " << new_client << " is trying to register" << std::endl;

}

// This function must be in channel
void IRCServer::__broadcast_message (int client, const std::string& msg) {
    UserBySocketIter it = user_table.tsbegin();

    while (it != user_table.tsend()) {
        User to_send = user_table.get_user(it->first);
        ++it;
        if (client != to_send.get_socket_fd() && to_send.get_is_auth())
            send(to_send.get_socket_fd(), msg.c_str(), msg.size(), 0);
    }
}

void IRCServer::__user_disconnect (int client) {
    close(client);
    eventhandler.unsubscribe_get(client);
    user_table.remove_user(client);
    user_msg_buffer.erase(client);
    std::cout << "Client " << client << " disconnected" << std::endl;
}

void IRCServer::__message_execution (int client, std::string& message) {
    Helpers::trim(message);
    if (message.empty())
        return;

    std::vector<std::string> tokens = Helpers::parse_msg(message);

    executor.execute(client, tokens);
}

void IRCServer::__message_checking (int client) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(client, buffer, BUFFER_SIZE, 0);

    if (bytes_received <= 0) {
        if (bytes_received == 0 || (errno != EAGAIN && errno != EWOULDBLOCK))
            __user_disconnect(client);
        return;
    }
    user_msg_buffer[client].append(buffer, bytes_received);
    
    std::string& buf = user_msg_buffer[client];
    std::size_t pos;
    std::string input;
    while ((pos = buf.find("\r\n")) != std::string::npos) {
        input = buf.substr(0, pos);
        buf.erase(0, pos + 2);
        __message_execution(client, input);
    }
}

const std::string& IRCServer::getPassword() const {
    return password;
}

UserTable& IRCServer::getUserTable() {
    return user_table;
}

Channel& IRCServer::getChannel(std::string chanel_name)
{
    return(channels[chanel_name]);
}