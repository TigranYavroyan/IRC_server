#include "EventHandler.hpp"

EventHandler::EventHandler () {
	clear_all_subscriptions();
}

EventHandler::~EventHandler () {}

bool EventHandler::is_get_event (int socket_fd) {
	return FD_ISSET(socket_fd, &ready_read);
}

bool EventHandler::is_send_event (int socket_fd) {
	return FD_ISSET(socket_fd, &ready_write);
}

int EventHandler::wait_event () {
	ready_read = read_subscriptors;
	ready_write = write_subscriptors;
	return select(max_fd + 1, &ready_read, &ready_write, NULL, NULL);
}

void EventHandler::subscribe_get (int socket_fd) {
	FD_SET(socket_fd, &read_subscriptors);
	max_fd = std::max(max_fd, socket_fd);
}

void EventHandler::subscribe_send (int socket_fd) {
	FD_SET(socket_fd, &write_subscriptors);
	max_fd = std::max(max_fd, socket_fd);
}

void EventHandler::unsubscribe_get (int socket_fd) {
	FD_CLR(socket_fd, &read_subscriptors);
	if (socket_fd == max_fd) {
        for (int i = max_fd - 1; i >= 0; --i) {
            if (FD_ISSET(i, &read_subscriptors) || FD_ISSET(i, &write_subscriptors)) {
                max_fd = i;
                return;
            }
        }
        max_fd = 0;
    }
}

void EventHandler::unsubscribe_send (int socket_fd) {
	FD_CLR(socket_fd, &write_subscriptors);
	if (socket_fd == max_fd) {
        for (int i = max_fd - 1; i >= 0; --i) {
            if (FD_ISSET(i, &write_subscriptors) || FD_ISSET(i, &read_subscriptors)) {
                max_fd = i;
                return;
            }
        }
        max_fd = 0;
    }
}

void EventHandler::clear_all_subscriptions () {
	FD_ZERO(&ready_read);
	FD_ZERO(&read_subscriptors);
	FD_ZERO(&write_subscriptors);
	FD_ZERO(&ready_write);
	max_fd = 0;
}