#include <Replies.hpp>

std::string Replies::crlf () {
	return "\r\n";
}

std::string Replies::connected (cref_str nickname) {
	return ": 001 " + nickname + " : Welcome to the IRC server!" + crlf();
}

std::string Replies::umodeIs (cref_str hostname, cref_str channelname, cref_str mode, cref_str user) {
	return	":" + hostname + " MODE " + channelname + " " + mode + " " + user + crlf();
}

std::string Replies::creationOnTime (cref_str nickname, cref_str channelname, cref_str creationtime) {
	return ": 329 " + nickname + " #" + channelname + " " + creationtime + crlf();
}

std::string Replies::channelModes (cref_str nickname, cref_str channelname, cref_str modes) {
	return ": 324 " + nickname + " #" + channelname + " " + modes + crlf();
}

std::string Replies::changeMode (cref_str hostname, cref_str channelname, cref_str mode, cref_str arguments) {
	return ":" + hostname + " MODE #" + channelname + " " + mode + " " + arguments + crlf();
}

std::string Replies::nickChange (cref_str oldnickname, cref_str nickname) {
	return ":" + oldnickname + " NICK " + nickname + crlf();
}

std::string Replies::joinMsg (cref_str hostname, cref_str ipaddress, cref_str channelname) {
	return ":" + hostname + "@" + ipaddress + " JOIN #" + channelname + crlf();
}

std::string Replies::namReply (cref_str nickname, cref_str channelname, cref_str clientslist) {
	return ": 353 " + nickname + " @ #" + channelname + " :" + clientslist + crlf();
}

std::string Replies::endOfNames (cref_str nickname, cref_str channelname) {
	return ": 366 " + nickname + " #" + channelname + " :END of /NAMES list" + crlf();
}

std::string Replies::topicIs (cref_str nickname, cref_str channelname, cref_str topic) {
	return ": 332 " + nickname + " #" +channelname + " :" + topic + crlf();
}

// ///////// ERRORS ////////////////

std::string Replies::err_needModeParm(cref_str channelname, cref_str mode) {
	return ": 696 #" + channelname + " * You must specify a parameter for the key mode. " + mode + crlf();
}

std::string Replies::err_invaliDModeParm(cref_str channelname, cref_str mode) {
	return ": 696 #" + channelname + " Invalid mode parameter. " + mode + crlf();
}

std::string Replies::err_keySet(cref_str channelname) {
	return ": 467 #" + channelname + " Channel key already set. " + crlf();
}

std::string Replies::err_unknownMode(cref_str nickname, cref_str channelname, cref_str mode) {
	return ": 472 " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + crlf();
}

std::string Replies::err_notenoUGHPARAM(cref_str nickname) {
	return ": 461 " + nickname + " :Not enough parameters." + crlf();
}

std::string Replies::err_channelNotFound(cref_str nickname, cref_str channelname) {
	return ": 403 " + nickname + " " + channelname + " :No such channel" + crlf();
}

std::string Replies::err_notopeRATOR(cref_str channelname) {
	return ": 482 #" + channelname + " :You're not a channel operator" + crlf();
}

std::string Replies::err_noSuchNick(cref_str channelname, cref_str name) {
	return ": 401 #" + channelname + " " + name + " :No such nick/channel" + crlf();
}

std::string Replies::err_incorpass(cref_str nickname) {
	return ": 464 " + nickname + " :Password incorrect !" + crlf();
}

std::string Replies::err_alreadyRegistered(cref_str nickname) {
	return ": 462 " + nickname + " :You may not reregister !" + crlf();
}

std::string Replies::err_noNickName(cref_str nickname) {
	return ": 431 " + nickname + " :No nickname given" + crlf();
}

std::string Replies::err_nickInUse(cref_str nickname) {
	return ": 433 " + nickname + " :Nickname is already in use" + crlf();
}

std::string Replies::err_oneUsNick(cref_str nickname) {
	return ": 432 " + nickname + " :Erroneus nickname" + crlf();
}

std::string Replies::err_notRegistered(cref_str nickname) {
	return ": 451 " + nickname + " :You have not registered!" + crlf();
}

std::string Replies::err_cmdnotFound(cref_str nickname, cref_str command) {
	return ": 421 " + nickname + " " + command + " :Unknown command" + crlf();
}
