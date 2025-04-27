#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <sys/select.h>
#include <cstddef>
#include <algorithm>

class EventHandler {
	fd_set read_subscriptors;
	fd_set ready_read;
	int max_fd;
public:
	EventHandler();
	~EventHandler();
public:
	bool is_new_event (int socket_fd);
	int wait_event();
	void subscribe (int socket_fd);
	void unsubscribe (int socket_fd);
	void clear_subscriptions () throw();
};

#endif // EVENTHANDLER_HPP