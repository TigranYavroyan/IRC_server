#include <sys/socket.h>
#include <netinet/in.h>

int server_fd;
int opt;
int new_client;
struct sockaddr_in server_addr;
const int MAX_PENDING_CONNECTIONS;
struct sockaddr_in client_addr;
socklen_t addr_len;

int main ()
{

/**
 * @param connection_protocol AF_INET(IPv4)
 * @param transfer_protocol SOCK_STREAM(TCP)
 * @param transfer_protocol_variation (default setup for TCP protocol)
 * 
 * $ creating the socket
 * $ socket is a file
*/
server_fd = socket(AF_INET, SOCK_STREAM, 0);

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
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

/**
 * @param socket_fd
 * @param GSA (generic_socket_address)
 * @param sizeof_GSA
 * 
 * $ Binding the socket_fd to the generic address
 * $ We need this generic approach , because there are many domain-specific addresses
*/
bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

/**
 * @param server_fd for listening the port defined after binding
 * @param backlog the maximum value of pending connections until the accepting
 * 
 * $ server_fd is a passive socket, which listenning on setuped IP:PORT
*/
listen(server_fd, MAX_PENDING_CONNECTIONS);

/**
 * @param server_fd , passive socket , which listening some IP : PORT
 * @param GSA
 * @param sizeof_GSA
 * 
 * $ accept returning active sockets fd, which is the fd of the client side 
*/
new_client = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
}

/**
 * 
 *  definitely lost: 0 bytes in 0 blocks
 *  ==54428==    indirectly lost: 0 bytes in 0 blocks
 *   ==54428==      possibly lost: 0 bytes in 0 blocks
 *  ==54428==    still reachable: 76,224 bytes in 30 blocks
 *   ==54428==         suppressed: 0 bytes in 0 blocks
 *
 * 
 * 
 * 
*/