#include "Channel.hpp"
#include "User.hpp"
#include <iostream>
#include <set>
#include <string>
#include <Replies.hpp>

Channel::Channel(const std::string& channel_name) 
    : name(channel_name), topic(""), key(""), user_limit(0), invite_only(false), topic_restricted(true)
{}

void Channel::setName(const std::string& _name) {
    name = _name;
}

bool Channel::addUser(User* user, std::string& err_msg, const std::string& provided_key) {
    if (invite_only) {
        std::set<User*>::iterator it = invited.find(user);
        if (it == invited.end()) {
            err_msg = Replies::err_cannotJoin("473", user->get_nickname(), name, "(+i)");
            return false;
        }
    }
    
    if (!key.empty() && key != provided_key)  {
        err_msg = Replies::err_cannotJoin("475", user->get_nickname(), name, "(+k)");
        return false;
    }

    if (user_limit > 0 && users.size() >= static_cast<size_t>(user_limit)) {
        err_msg = Replies::err_cannotJoin("471", user->get_nickname(), name, "(+l)");
        return false;
    }

    users.insert(user);
    if (invite_only) {
        invited.erase(user);
    }
    if (users.size() == 1) 
        addOperator(user);

    return true;
}

void Channel::removeUser(User* user) {
    users.erase(user);
    operators.erase(user);
    invited.erase(user);
}

bool Channel::hasUser(User* user) const {
    return users.find(user) != users.end();
}

void Channel::addOperator(User* user) {
    if (hasUser(user)) {
        operators.insert(user);
    }
}

void Channel::removeOperator(User* user) {
    operators.erase(user);
}

bool Channel::isOperator(User* user) const {
    return operators.find(user) != operators.end();
}

void Channel::inviteUser(User* user) {
    invited.insert(user);
}

bool Channel::isInvited(User* user) const {
    return invited.find(user) != invited.end();
}


void Channel::setTopic(const std::string& new_topic) {
    topic = new_topic;
}

std::string Channel::getTopic() const {
    return topic;
}

void Channel::broadcast(const std::string& message, const User* sender) {
    std::set<User*>::iterator it = users.begin();
    while (it != users.end()) {
        if (*it != sender) {
            (*it)->sendMessage(message);
        }
        ++it;
    }
}

void Channel::setInviteOnly(bool mode) {
    invite_only = mode;
}

void Channel::setTopicRestricted(bool mode) {
    topic_restricted = mode;
}

void Channel::setKey(const std::string& new_key) {
    key = new_key;
}

void Channel::removeKey() {
    key = "";
}

void Channel::setUserLimit(int limit) {
    user_limit = limit;
}

void Channel::removeUserLimit() {
    user_limit = 0;
}

const std::string& Channel::getName() const {
    return name;
}

bool Channel::getInviteOnly() const {
    return invite_only;
}

bool Channel::getTopicRestricted() const {
    return topic_restricted;
}

bool Channel::hasKey() const {
    return !key.empty();
}

std::string Channel::getKey() const {
    return key;
}

int Channel::getUserLimit() const {
    return user_limit;
}

size_t Channel::getUserCount() const {
    return users.size();
}

User* Channel::getUserByNick(const std::string& nick)
{
    std::set<User*>::iterator it = this->users.begin();
    for (; it != this->users.end(); ++it)
    {
        if ((*it)->get_nickname() == nick)
            return *it;
    }
    return NULL;
}

std::string Channel::getUserList() const
{
    std::string result;
    for (std::set<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        if (operators.find(*it) != operators.end()) {
            result += '@';
        }
        if (*it)
            result += (*it)->get_nickname() + " ";
    }
    if (!result.empty())
        result.erase(result.size() - 1);
    return result;
}

std::string Channel::getModes () const {
    std::string modes;

    if (!key.empty()) {
        modes.push_back('k');
    }

    if (user_limit) {
        modes.push_back('l');
    }

    if (topic_restricted) {
        modes.push_back('t');
    }

    if (invite_only) {
        modes.push_back('i');
    }

    return modes;
}
