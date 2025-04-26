#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include <User.hpp>
#include <set>

class Channel {
	std::map<std::string, User> users;
	bool is_invite_only;
	std::string name;
	std::set<User> operators;
	std::string password;
	std::string topic;
public:
};

#endif // CHANNEL_HPP