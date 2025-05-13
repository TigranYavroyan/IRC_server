#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include <map>
#include <vector>
#include <User.hpp>
#include <UserTable.hpp>

class User;

class Channel {
private:
    std::string name;
    std::string topic;
    std::string key;             // For +k mode (channel password)
    int user_limit;              // For +l mode
    bool invite_only;            // +i mode
    bool topic_restricted;       // +t mode

    std::set<User*> users;
    std::set<User*> operators;
    std::set<User*> invited;

public:
    Channel(const std::string& channel_name);

    bool addUser(User* user, const std::string& provided_key = "");
    void removeUser(User* user);
    bool hasUser(User* user) const;

    void addOperator(User* user);
    void removeOperator(User* user);
    bool isOperator(User* user) const;

    void inviteUser(User* user);
    bool isInvited(User* user) const;

    void setTopic(const std::string& new_topic);
    std::string getTopic() const;

    void broadcast(const std::string& message, User* sender = NULL);

    void setInviteOnly(bool mode);
    void setTopicRestricted(bool mode);
    void setKey(const std::string& new_key);
    void removeKey();
    void setUserLimit(int limit);
    void removeUserLimit();

    const std::string& getName() const;
    bool getInviteOnly() const;
    bool getTopicRestricted() const;
    bool hasKey() const;
    std::string getKey() const;
    int getUserLimit() const;
    size_t getUserCount() const;

	User* getUserByNick(const std::string& nick);

};

#endif