#include <Executor.hpp>
#include <IRCServer.hpp>

void Executor::__create_cmds_table (IRCServer& server) {
	std::pair<std::string, ACommand*> init_table[] = {
		std::make_pair("PASS", new Pass(server)),
		std::make_pair("NICK", new Nick(server)),
		std::make_pair("USER", new UserCommand(server)),
		std::make_pair("JOIN", new Join(server)),
		std::make_pair("PART", new Part(server)),
		std::make_pair("PRIVMSG", new PrivMsg(server)),
		std::make_pair("PING", new Ping(server)),
		std::make_pair("PONG", new Pong(server)),
		std::make_pair("QUIT", new Quit(server))
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
	User& client = user_table[socket_fd];
	bool can_welcome;

	// separate method
	if (!(client.get_is_auth())) {
		if (cmd != "PASS")
		{
			std::cout << "mtav" << std::endl;
			std::string err_msg = Replies::err_notRegistered(cmd, "");
            send(socket_fd, err_msg.c_str(), err_msg.size(), 0);
			return;
		}
	}

	if (client.get_is_auth() && (client.get_nickname().empty() || client.get_username().empty())) {
		if (cmd != "NICK" && cmd != "USER")
        {
			std::cout << "mtav2" << std::endl;
			std::string err_msg = Replies::err_notRegistered(cmd, "");
            send(socket_fd, err_msg.c_str(), err_msg.size(), 0);
			return;
        }
    }

	
	std::map<std::string, ACommand*>::const_iterator it = commands_table.find(cmd);
	if (it == commands_table.end()) {
		std::string err_msg = Replies::err_cmdnotFound(client.get_nickname(), cmd);
		send(socket_fd, err_msg.c_str(), err_msg.size(), 0);
		return;
	}
	
	it->second->execute(socket_fd, tokens);

	// This must be fixed, just get at the of this reference
	can_welcome = !client.get_nickname().empty() && !client.get_username().empty();

	if (can_welcome && !client.get_is_get_welcome_msg()) {
		client.set_is_get_welcome_msg();
		std::string welcome_msg = Replies::connected(client.get_nickname());
		send(socket_fd, welcome_msg.c_str(), welcome_msg.size(), 0);
	}
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