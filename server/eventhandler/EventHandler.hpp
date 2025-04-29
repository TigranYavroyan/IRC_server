#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <sys/select.h>
#include <cstddef>
#include <algorithm>

class EventHandler {
	fd_set write_subscriptors;
	fd_set ready_write;
	fd_set read_subscriptors;
	fd_set ready_read;
	int max_fd;
public:
	EventHandler();
	~EventHandler();
public:
	bool is_get_event (int socket_fd);
	bool is_send_event (int socket_fd);
	int wait_event();
	void subscribe_get (int socket_fd);
	void unsubscribe_get (int socket_fd);
	void subscribe_send (int socket_fd);
	void unsubscribe_send (int socket_fd);
	void clear_all_subscriptions ();
};

#endif // EVENTHANDLER_HPP