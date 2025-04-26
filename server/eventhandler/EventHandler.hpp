#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <sys/select.h>

class EventHandler {
	fd_set all_fds;
	fd_set read_fd;
public:
	bool is_new_event (int socket_fd);
};

#endif // EVENTHANDLER_HPP