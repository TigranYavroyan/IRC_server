#include "EventHandler.hpp"

EventHandler::EventHandler () {
	clear_subscriptions();
}

EventHandler::~EventHandler () {}

bool EventHandler::is_new_event (int socket_fd) {
	return FD_ISSET(socket_fd, &ready_read);
}

int EventHandler::wait_event () {
	ready_read = read_subscriptors;
	return select(max_fd + 1, &ready_read, NULL, NULL, NULL);
}

void EventHandler::subscribe (int socket_fd) {
	FD_SET(socket_fd, &read_subscriptors);
	max_fd = std::max(max_fd, socket_fd);
}

void EventHandler::unsubscribe (int socket_fd) {
	FD_CLR(socket_fd, &read_subscriptors);
	if (socket_fd == max_fd) {
        for (int i = max_fd - 1; i >= 0; --i) {
            if (FD_ISSET(i, &read_subscriptors)) {
                max_fd = i;
                return;
            }
        }
        max_fd = 0;
    }
}

void EventHandler::clear_subscriptions () throw() {
	FD_ZERO(&ready_read);
	FD_ZERO(&read_subscriptors);
	max_fd = 0;
}