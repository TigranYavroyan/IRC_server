template <port_id PORT>
IRC_server<PORT>::~IRC_server () {
    close_connection_all();
}

template <port_id PORT>
void IRC_server<PORT>::close_connection_all () {
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
     * @param backlog the maximum value of pending connection until the accepting
    */
    const int MAX_PENDING_CONNECTIONS = 2;
    if (listen(server_fd, MAX_PENDING_CONNECTIONS)) {
        perror("listen");
        _exit(1);
    }

    std::cout << "Listening on " << INADDR_ANY << ":" << PORT << "..." << std::endl;
}