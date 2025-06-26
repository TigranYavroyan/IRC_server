#include <IRCServer.hpp>
#include <Logger.hpp>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>

#ifdef DEBUG
    #include <Debugger.hpp>
#endif

IRCServer::IRCServer (int port, const std::string& _password): PORT(port), password(_password) {
    executor.set_server(*this);
    std::time_t now = std::time(NULL);
    creation_time = std::ctime(&now);
    creation_time.erase(creation_time.end() - 1);
}


IRCServer::~IRCServer () {
    closeConnectionAll();
}

std::string IRCServer::get_creation_time() const {
    return creation_time;
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

    Logger::server_on(PORT, password);
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

const std::string& IRCServer::getPassword() const {
    return password;
}

UserTable& IRCServer::getUserTable() {
    return user_table;
}

bool IRCServer::is_channel_exist (const std::string& channel_name) {
    return channels.count(channel_name);
}

Channel& IRCServer::getChannel(const std::string& channel_name) {
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);

    if (it == channels.end()) {
        channels.insert(std::pair<std::string, Channel>(channel_name, Channel(channel_name)));
    }

    return channels[channel_name];
}

void IRCServer::removeFromChannel (User& user, const std::string& channel_name) {
    Channel& channel = channels[channel_name];
    channel.removeUser(&user);
}

void IRCServer::removeChannel (const std::string& channel_name) {
    channels.erase(channel_name);

    #ifdef DEBUG
        Debugger::channel_removed(channel_name);
    #endif
}

void IRCServer::removeFromAllChannels (User& user, const std::string& msg) {
    std::set<std::string>::iterator begin = user.ch_begin();
    std::set<std::string>::iterator end = user.ch_end();

    while (begin != end) {
        Channel& ch = getChannel(*begin);
        ch.removeUser(&user);
        ch.broadcast(msg);
        if (!ch.getUserCount()) {
			removeChannel(*begin);
		}
        ++begin;
    }
    user.exit_from_all_channels();
}

void IRCServer::removeFromAllChannels (User& user) {
    std::set<std::string>::iterator begin = user.ch_begin();
    std::set<std::string>::iterator end = user.ch_end();
    std::string msg;

    while (begin != end) {
        Channel& ch = getChannel(*begin);
        ch.removeUser(&user);
        msg = Replies::partMsg(user, ch.getName());
        ch.broadcast(msg);

        ++begin;
    }

    user.exit_from_all_channels();
}

void IRCServer::disconnectClient (User& user) {
    __user_disconnect(user.get_socket_fd());
}


// ----------------------------- Private methods ---------------------------------

void IRCServer::__accept_connection () {
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    std::memset(&client_addr, 0, addr_len);

    int new_client = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    
    if (new_client < 0) {
        throw IRC::exception(std::strerror(errno));
    }
    
    std::string hostname = inet_ntoa(client_addr.sin_addr);

    user_table.set_user(new_client, hostname);
    user_msg_buffer[new_client] = "";
    eventhandler.subscribe_get(new_client);

    Logger::client_try_to_connect(new_client);
}

void IRCServer::__user_disconnect (int client) {
    eventhandler.unsubscribe_get(client);
    eventhandler.unsubscribe_send(client);
    user_msg_buffer.erase(client);
    user_table.remove_user(client);
    close(client);
    Logger::client_disconnected(client);
}

bool IRCServer::__message_execution (int client, std::string& message) {
    Helpers::trim(message);
    if (message.empty())
        return true;

    std::vector<std::string> tokens = Helpers::parse_msg(message);

    return executor.execute(client, tokens);
}

void IRCServer::__message_checking (int client) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(client, buffer, BUFFER_SIZE, 0);

    if (bytes_received <= 0) {
        User& user = user_table[client];
        std::string msg = Replies::quitMsg(user);
        removeFromAllChannels(user, msg);
        disconnectClient(user);
        return;
    }
    user_msg_buffer[client].append(buffer, bytes_received);
    
    std::string& buf = user_msg_buffer[client];
    std::size_t pos;
    std::string input;
    while ((pos = buf.find("\r\n")) != std::string::npos) {
        input = buf.substr(0, pos);
        buf.erase(0, pos + 2);

        // ! if client disconnected
        if (!__message_execution(client, input)) {
            break;
        }
    }
}