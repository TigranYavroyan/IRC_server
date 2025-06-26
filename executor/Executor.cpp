#include <Executor.hpp>
#include <IRCServer.hpp>
#include <Logger.hpp>

#ifdef DEBUG
	#include <Debugger.hpp>
#endif

void Executor::__create_cmds_table (IRCServer& server) {
	std::pair<std::string, ACommand*> init_table[] = {
		std::make_pair("PASS", new Pass(server)),
		std::make_pair("NICK", new Nick(server)),
		std::make_pair("USER", new UserCommand(server)),
		std::make_pair("JOIN", new Join(server)),
		std::make_pair("PART", new Part(server)),
		std::make_pair("PRIVMSG", new PrivMsg(server)),
		std::make_pair("PING", new Ping(server)),
		std::make_pair("QUIT", new Quit(server)),
		std::make_pair("MODE", new Mode(server)),
		std::make_pair("TOPIC", new Topic(server)),
		std::make_pair("KICK", new Kick(server)),
		std::make_pair("INVITE", new Invite(server))
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

bool Executor::execute (int socket_fd, const std::vector<std::string>& tokens) const {
	UserTable& user_table = server->getUserTable();
	std::string cmd = tokens[0];
	User& client = user_table[socket_fd];
	bool can_register;

	#ifdef DEBUG
		Debugger::print_input(tokens);
	#endif

	if (__cap_ls_handling(client, tokens))
		return true;

	if (is_registration_done(client, cmd)) {
		std::string err_msg = Replies::err_notRegistered(cmd, client.get_nickname());
		client.sendMessage(err_msg);
		return true;
	}
	
	std::map<std::string, ACommand*>::const_iterator it = commands_table.find(cmd);
	if (it == commands_table.end()) {
		std::string err_msg = Replies::err_cmdnotFound(client.get_nickname(), cmd);
		client.sendMessage(err_msg);
		return true;
	}
	
	it->second->execute(socket_fd, tokens);
	
	// $ if quit, don't do anything
	if (tokens[0] == "QUIT")
		return false;

	can_register = client.is_nick() && !client.is_user();

	if (can_register && !client.get_is_registered()) {
		client.set_is_registered();
		Logger::client_registered(socket_fd);
		client.sendMessage(Replies::connected(client.get_nickname()));
		client.sendMessage(Replies::yourHost(client.get_nickname()));
		client.sendMessage(Replies::created(client.get_nickname(), server->get_creation_time()));
		client.sendMessage(Replies::myInfo(client.get_nickname()));
	}

	return true;
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

bool Executor::__cap_ls_handling (const User& client, const std::vector<std::string>& tokens) const {
	if (tokens.empty() || tokens.size() < 2)
		return false;
	
	if (tokens[0] == "CAP") {
		if (tokens[1] == "LS") {
			const std::string msg = Replies::cap_ls();
			send(client.get_socket_fd(), msg.c_str(), msg.size(), 0);
		}
		return true;
	}
	return false;
}

bool Executor::is_registration_done (const User& client, const std::string& cmd) const {
	#ifdef DEBUG
		if (cmd == "PASS") {
			Debugger::client_pass(client);
		}
		else if (cmd == "NICK") {
			Debugger::client_nick(client);
		}
		else if (cmd == "USER") {
			Debugger::client_user(client);
		}
	#endif
	
	if (cmd == "QUIT")
		return false;

	if (!(client.get_is_auth()) && (cmd != "PASS"))
		return true;

	if (client.get_is_auth() && (!client.is_nick() || client.is_user())) {
		if (cmd != "NICK" && cmd != "USER")
			return true;
    }

	return false;
}