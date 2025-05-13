#include <Executor.hpp>
#include <IRCServer.hpp>

void Executor::__create_cmds_table (IRCServer& server) {
	std::pair<std::string, ACommand*> init_table[] = {
		std::make_pair("pass", new Pass(server)),
		std::make_pair("join", new Join(server)),
		std::make_pair("nick", new Nick(server)),
		std::make_pair("part", new Part(server)),
		std::make_pair("privmsg", new PrivMsg(server)),
		std::make_pair("quit", new Quit(server))
	};

	commands_table = std::map<std::string, ACommand*> (
		init_table,
		init_table + sizeof(init_table) / sizeof(init_table[0])
	);
}

void Executor::set_server (IRCServer& _server) {
	server = &_server;

	clear_cmds();
	__create_cmds_table(*server);
}

Executor::Executor (IRCServer& _server): server(&_server) {
	__create_cmds_table(*server);
}

void Executor::execute (int socket_fd, const std::vector<std::string>& tokens) const {
	UserTable& user_table = server->getUserTable();
	std::string cmd = tokens[0];
	User client = user_table.get_user(socket_fd);
	std::cout << "Execute called" << std::endl;
	
	if (!(client.get_is_auth()) || client.get_nickname().empty()) {
		if (cmd != "pass" && cmd != "nick")
        {
			std::string err_msg = Replies::err_notRegistered("");
            send(socket_fd, err_msg.c_str(), err_msg.size(), 0);
			return;
        }
    }

	std::map<std::string, ACommand*>::const_iterator it = commands_table.find(cmd);
	if (it == commands_table.end()) {
		std::cout << "Unknown command: " << tokens[0] << std::endl;
		return;
	}
	
	it->second->execute(socket_fd, tokens);
}

void Executor::clear_cmds () {
	if (commands_table.empty())
		return;

	std::map<std::string, ACommand*>::iterator it = commands_table.begin();

	while (it != commands_table.end()) {
		delete (it->second);
		++it;
	}

	commands_table.clear();
}

Executor::~Executor () {
	clear_cmds();
}