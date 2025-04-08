template <port_id PORT>
IRC_server<PORT>::~IRC_server () {
    closeConnectionAll();
}

template <port_id PORT>
void IRC_server<PORT>::closeConnectionAll () {
    close(server_fd);
    for (int client : clients) {
        close(client);
    }
}


template <port_id PORT>
void IRC_server<PORT>::setupServer () {
    /**
     * @param connection_protocol AF_INET(IPv4)
     * @param transfer_protocol SOCK_STREAM(TCP)
     * @param transfer_protocol_variation (default setup for TCP protocol)
     * 
     * $ creating the socket
     * $ socket is a file
    */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        _exit(1);
    }

    struct sockaddr_in server_addr;
    int opt = 1;

    /**
     * @param socket_fd
     * @param abstraction_level this is for working with socket in which layer (this is the socket API levelSOL_SOCKET)
     * @param opt_for_working_with_port for port configuration
     * @param flag
     * @param sizeof_the_flag
     * 
     * $ setting the sockets additional options
     * ! this is optional, but recommended to use
    */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        _exit(1);
    }
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // the IP
    server_addr.sin_port = htons(PORT); // the PORT

    /**
     * @param socket_fd
     * @param GSA (generic_socket_address)
     * @param sizeof_GSA
     * 
     * $ Binding the socket_fd to the generic address
     * $ We need this generic approach , because there are many domain-specific addresses
    */
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        perror("bind");
        _exit(1);
    }

    /**
     * @param server_fd for listening the port defined after binding
     * @param backlog the maximum value of pending connections until the accepting
     * 
     * $ server_fd is a passive socket, which listenning on setuped IP:PORT
    */
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
void IRC_server<PORT>::acceptConnection () {
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    /**
     * @param server_fd , passive socket , which listening some IP : PORT
     * @param GSA
     * @param sizeof_GSA
     * 
     * $ accept returning active sockets fd, which is the fd of the client side 
    */
    int new_client = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (new_client < 0) {
        perror("accept");
        exit(1);
    }

    clients.push_back(new_client);

    /**
     * @param socket_fd
     * @param fd_set (int this case all_fdset representing all connected users and servers)
    */
    FD_SET(new_client, &all_fdset);
    max_fd = std::max(max_fd, new_client);

    std::cout << "New client connected: " << new_client << "\n";
    std::string welcome_msg = "Welcome to the IRC-like chat server!\n";
    if (send(new_client, welcome_msg.c_str(), welcome_msg.size(), 0) < 0)
        perror("send");
}
