#ifndef ALIASES_HPP
#define ALIASES_HPP

#include <map>
#include <string>
#include <User.hpp>

typedef std::map<int, User*> UserBySocket;
typedef std::map<std::string, User*> UserByName;
typedef UserBySocket::iterator UserBySocketIter;
typedef UserByName::iterator UserByNameIter;

#endif // ALIASES_HPP