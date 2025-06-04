#include <Replies.hpp>

std::string Replies::server_name = "irc.42.chat";

std::string Replies::crlf () {
	return "\r\n";
}

std::string Replies::connected (cref_str nickname) {
	return ":" + server_name + " 001 " + nickname + " : Welcome to the IRC server!" + crlf();
}

std::string Replies::umodeIs (cref_str hostname, cref_str channelname, cref_str mode, cref_str user) {
	return ":" + server_name	+ "" + hostname + " MODE " + channelname + " " + mode + " " + user + crlf();
}

std::string Replies::creationOnTime (cref_str nickname, cref_str channelname, cref_str creationtime) {
	return ":" + server_name + " 329 " + nickname + " #" + channelname + " " + creationtime + crlf();
}

std::string Replies::channelModes (cref_str nickname, cref_str channelname, cref_str modes) {
	return ":" + server_name + " 324 " + nickname + " #" + channelname + " " + modes + crlf();
}

std::string Replies::changeMode (cref_str hostname, cref_str channelname, cref_str mode, cref_str arguments) {
	return ":" + server_name + "" + hostname + " MODE #" + channelname + " " + mode + " " + arguments + crlf();
}

std::string Replies::nickChange (cref_str oldnickname, cref_str nickname) {
	return ":" + server_name + "" + oldnickname + " NICK " + nickname + crlf();
}

std::string Replies::joinMsg (cref_str hostname, cref_str ipaddress, cref_str channelname) {
	return ":" + server_name + "" + hostname + "@" + ipaddress + " JOIN #" + channelname + crlf();
}

std::string Replies::namReply (cref_str nickname, cref_str channelname, cref_str clientslist) {
	return ":" + server_name + " 353 " + nickname + " @ #" + channelname + " :" + clientslist + crlf();
}

std::string Replies::endOfNames (cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 366 " + nickname + " #" + channelname + " :END of /NAMES list" + crlf();
}

std::string Replies::topicIs (cref_str nickname, cref_str channelname, cref_str topic) {
	return ":" + server_name + " 332 " + nickname + " #" +channelname + " :" + topic + crlf();
}

// ///////// ERRORS ////////////////

std::string Replies::err_needModeParm(cref_str command_name, cref_str channelname, cref_str mode) {
	return ":" + server_name + " 696 #" + command_name + " " + channelname + " * You must specify a parameter for the key mode. " + mode + crlf();
}

std::string Replies::err_invaliDModeParm(cref_str command_name, cref_str channelname, cref_str mode) {
	return ":" + server_name + " 696 #" + command_name + " " + channelname + " Invalid mode parameter. " + mode + crlf();
}

std::string Replies::err_keySet(cref_str command_name, cref_str channelname) {
	return ":" + server_name + " 467 #" + command_name + " " + channelname + " Channel key already set. " + crlf();
}

std::string Replies::err_unknownMode(cref_str command_name, cref_str nickname, cref_str channelname, cref_str mode) {
	return ":" + server_name + " 472 " + command_name + " " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + crlf();
}

std::string Replies::err_notEnoughParam(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 461 " + command_name + " " + nickname + " :Not enough parameters." + crlf();
}

std::string Replies::err_channelNotFound(cref_str command_name, cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 403 " + command_name + " " + nickname + " " + channelname + " :No such channel" + crlf();
}

std::string Replies::err_notopeRATOR(cref_str command_name, cref_str channelname) {
	return ":" + server_name + " 482 #" + command_name + " " + channelname + " :You're not a channel operator" + crlf();
}

std::string Replies::err_noSuchNick(cref_str command_name, cref_str channelname, cref_str name) {
	return ":" + server_name + " 401 #" + command_name + " " + channelname + " " + name + " :No such nick/channel" + crlf();
}

std::string Replies::err_incorpass(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 464 " + command_name + " " + nickname + " :Password incorrect !" + crlf();
}

std::string Replies::err_alreadyRegistered(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 462 " + command_name + " " + nickname + " :You may not reregister !" + crlf();
}

std::string Replies::err_noNickName(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 431 " + command_name + " " + nickname + " :No nickname given" + crlf();
}

std::string Replies::err_nickInUse(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 433 " + command_name + " " + command_name + " " + nickname + " :Nickname is already in use" + crlf();
}

std::string Replies::err_oneUsNick(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 432 " + command_name + " " + nickname + " :Erroneus nickname" + crlf();
}

std::string Replies::err_notRegistered(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 451 " + command_name + " " + nickname + " :You have not registered!" + crlf();
}

std::string Replies::err_cmdnotFound(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 421 " + command_name + " " + nickname + " :Unknown command" + crlf();
}

std::string Replies::err_cannotJoin(cref_str command_name, cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 475 " + command_name + " " + nickname + " " + channelname + " :Cannot join channel (+k or +i or full)" + crlf();
}