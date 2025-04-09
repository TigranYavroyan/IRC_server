template <port_id PORT>
IRC_server<PORT>::~IRC_server () {
    closeConnectionAll();
}

template <port_id PORT>
void IRC_server<PORT>::closeConnectionAll () {
    close(server_fd);
    for (std::size_t i = 0; i < clients.size(); ++i) {
        close(clients[i]);
    }
}

template <port_id PORT>
void IRC_server<PORT>::setupServer () {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        _exit(1);
    }

    struct sockaddr_in server_addr;
    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        _exit(1);
    }
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // the IP
    server_addr.sin_port = htons(PORT); // the PORT

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        perror("bind");
        _exit(1);
    }

    const int MAX_PENDING_CONNECTIONS = 2;
    if (listen(server_fd, MAX_PENDING_CONNECTIONS)) {
        perror("listen");
        _exit(1);
    }

    /**
     * $ We need fd_sets for knowing is there something happened or not (someone connected or sent message) 
    */
    FD_ZERO(&all_fdset);
    FD_SET(server_fd, &all_fdset);
    max_fd = server_fd;

    std::cout << "Listening on " << INADDR_ANY << ":" << PORT << "..." << std::endl;
}


template <port_id PORT>
void IRC_server<PORT>::__acceptConnection () {
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int new_client = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (new_client < 0) {
        perror("accept");
        _exit(1);
    }

    clients.push_back(new_client);

    /**
     * @param socket_fd
     * @param fd_set (int this case all_fdset representing all connected users and servers)
     * 
     * $ we need the max_fd for select
    */
    FD_SET(new_client, &all_fdset);
    max_fd = std::max(max_fd, new_client);

    std::cout << "New client is trying to connect: " << new_client << "\n";
    std::string welcome_msg = "Welcome to the IRC-like chat server!\nEnter the password: ";
    if (send(new_client, welcome_msg.c_str(), welcome_msg.size(), 0) < 0)
        perror("send");
}

template <port_id PORT>
void IRC_server<PORT>::__right_trim (std::string& str, const char* delims) {
    str.erase(str.find_last_not_of(delims) + 1);
}

template <port_id PORT>
void IRC_server<PORT>::__left_trim (std::string& str, const char* delims) {
    str.erase(0, str.find_first_not_of(delims));
}

template <port_id PORT>
void IRC_server<PORT>::__trim (std::string& str, const char* delims) {
    __left_trim(str, delims);
    __right_trim(str, delims);
}

template <port_id PORT>
template <typename T>
std::string IRC_server<PORT>::to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
