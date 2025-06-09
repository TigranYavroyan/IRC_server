#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <vector>
#include <string>
#include <map>
#include <utility>

#include <Commands.hpp>

class IRCserver;

class Executor {
	IRCServer* server;
	std::map<std::string, ACommand*> commands_table;
public:
	Executor (): server(NULL) {}
	Executor (IRCServer& _server);
public:
	void set_server (IRCServer& _server);
	void execute (int socket_fd, const std::vector<std::string>& tokens) const;
	void clear_cmds ();
	bool is_registration_done (const User& client, const std::string& cmd) const;
	~Executor ();
private:
	bool __cap_ls_handling(const User& client, const std::vector<std::string>& tokens) const;
	void __create_cmds_table (IRCServer& server);
};

#endif // EXECUTOR_HPP