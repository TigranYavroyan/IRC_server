#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <vector>
#include <string>
#include <map>

#include <IRCServer.hpp>
#include <ACommand.hpp>

class Executor {
	std::vector<std::string> tokens;
	IRCServer& server;
	std::map<std::string, ACommand*> commands_table;
public:
	Executor (IRCServer& _server, const std::vector<std::string>& _tokens);
private:
	bool __is_command (const std::string& str);
};

#endif // EXECUTOR_HPP