#include <Replies.hpp>

std::string Replies::server_name = "irc.42.chat";

std::string Replies::crlf () {
	return "\r\n";
}

std::string Replies::cap_ls () {
	return "CAP * LS :" + crlf();
}

std::string Replies::userFullName (const User& user) {
	return ":" + user.get_nickname() + "!~" + user.get_username() + "@" + user.get_hostname();
}

std::string Replies::err_noRecipientGiven(cref_str nickname, cref_str command_name) {
	return ":" + server_name + " 421 " + nickname + " :No recipient given (" + command_name + ")" + crlf();
}

std::string Replies::privateMessage(const User& sender, cref_str target, cref_str message) {
	return userFullName(sender) + " PRIVMSG " + target + " :" + message + crlf();
}

std::string Replies::connected (cref_str nickname) {
	return ":" + server_name + " 001 " + nickname + " : Welcome to the IRC server!" + crlf();
}

std::string Replies::umodeIs (cref_str hostname, cref_str channelname, cref_str mode, cref_str user) {
	return ":" + server_name	+ "" + hostname + " MODE " + channelname + " " + mode + " " + user + crlf();
}

std::string Replies::creationOnTime (cref_str nickname, cref_str channelname, cref_str creationtime) {
	return ":" + server_name + " 329 " + nickname + " " + channelname + " " + creationtime + crlf();
}

std::string Replies::channelModes (cref_str nickname, cref_str channelname, cref_str inp_modes) {
	cref_str modes = "+n" + inp_modes;
	return ":" + server_name + " 324 " + nickname + " " + channelname + " " + modes + crlf();
}

std::string Replies::changeMode (cref_str hostname, cref_str channelname, cref_str mode, cref_str arguments) {
	return ":" + server_name + "" + hostname + " MODE #" + channelname + " " + mode + " " + arguments + crlf();
}

std::string Replies::message (const User& user, cref_str msg) {
	return userFullName(user) + " " + msg + crlf();
}

std::string Replies::nickChange (cref_str oldnickname, cref_str nickname) {
	return ":" + server_name + "" + oldnickname + " NICK " + nickname + crlf();
}

std::string Replies::quitMsg (const User& quited_user, cref_str quit_msg) {
	return userFullName(quited_user) + " QUIT :" + quit_msg + crlf();
}

std::string Replies::partMsg (const User& quited_user, cref_str channelname ,cref_str part_msg) {
	return userFullName(quited_user) + " PART " + channelname + " " + part_msg + crlf();
}

std::string Replies::joinMsg (const User& joined_user, cref_str channelname) {
	return userFullName(joined_user) + " JOIN " + channelname + crlf();
}

std::string Replies::namReply (cref_str nickname, cref_str channelname, cref_str clientslist) {
	return ":" + server_name + " 353 " + nickname + " @ " + channelname + " :" + clientslist + crlf();
}

std::string Replies::endOfNames (cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 366 " + nickname + " " + channelname + " :END of /NAMES list" + crlf();
}

std::string Replies::topicIs (cref_str nickname, cref_str channelname, cref_str topic) {
	return ":" + server_name + " 332 " + nickname + " " + channelname + " :" + topic + crlf();
}

std::string Replies::topicMsg (const User& user, cref_str channelname, cref_str topic) {
	return userFullName(user) + " TOPIC " + channelname + " :" + topic + crlf();
}

std::string Replies::inviteMsg (const User& user, cref_str recipient_name, cref_str chanellname) {
	return userFullName(user) + " INVITE " + recipient_name + " :" + chanellname + crlf();
}

std::string Replies::inviteMsgToSender (cref_str sender_nick, cref_str recipient_nick, cref_str channelname) {
	return ":" + server_name + " 341 " + sender_nick + " " + recipient_nick + " " + channelname + crlf();
}

std::string Replies::kickMsg (const User& kicker, cref_str channelname, cref_str target, cref_str kickmsg) {
	return userFullName(kicker) + " KICK " + channelname + " " + target + " :" + kickmsg + crlf();
}

std::string Replies::dccMessage (const User& sender, cref_str recipient_name, cref_str msg) {
	return userFullName(sender) + " PRIVMSG " + recipient_name + " :" + msg + crlf();
}

// ///////// ERRORS ////////////////

std::string Replies::err_needModeParm(cref_str command_name, cref_str channelname, cref_str mode) {
	return ":" + server_name + " 696 " + command_name + " " + channelname + " * You must specify a parameter for the key mode. " + mode + crlf();
}

std::string Replies::err_invaliDModeParm(cref_str nickname, char mode) {
	return ":" + server_name + " 501 " + nickname + " :Unknown MODE flag " + mode + crlf();
}

std::string Replies::err_unknownMode(cref_str command_name, cref_str nickname, cref_str channelname, cref_str mode) {
	return ":" + server_name + " 472 " + command_name + " " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + crlf();
}

std::string Replies::err_notEnoughParam(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 461 " + nickname + " " + command_name + " :Not enough parameters." + crlf();
}

std::string Replies::err_notOperator(cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 482 " + nickname + " " + channelname + " :You're not a channel operator" + crlf();
}

std::string Replies::err_noSuchNick(cref_str sender, cref_str recipient) {
	return ":" + server_name + " 401 " + sender + " " + recipient + " :No such nick/channel" + crlf();
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
	return ":" + server_name + " 433 " +  command_name + " " + nickname + " :Nickname is already in use" + crlf();
}

std::string Replies::err_oneUsNick(cref_str old_nick, cref_str new_nick) {
	return ":" + server_name + " 432 " + old_nick + " " + new_nick + " :Erroneus nickname" + crlf();
}

std::string Replies::err_notRegistered(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 451 " + command_name + " " + nickname + " :You have not registered!" + crlf();
}

std::string Replies::err_cmdnotFound(cref_str command_name, cref_str nickname) {
	return ":" + server_name + " 421 " + command_name + " " + nickname + " :Unknown command" + crlf();
}

std::string Replies::err_cannotJoin(cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 475 JOIN " + nickname + " " + channelname + " :Cannot join channel " + crlf();
}

std::string Replies::err_cannotJoin(cref_str err_code, cref_str nickname, cref_str channelname, cref_str mode) {
	return ":" + server_name + " " + err_code + " " + nickname + " " + channelname + " :Cannot join channel " + mode + crlf();
}

std::string Replies::err_noTextToSend(cref_str nickname) {
	return ":" + server_name + " 412 " + nickname + " :No text to send" + crlf();
}

std::string Replies::err_noOnThatChannel (cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 442 " + nickname + " " + channelname + " :You're not on that channel" + crlf();
}

std::string Replies::err_noSuchChannel (cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 403 " + nickname + " " + channelname + " :No such channel" + crlf();
}

std::string Replies::err_noTopicSet (cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 331 " + nickname + " " + channelname + " :No topic is set." + crlf();
}

std::string Replies::err_alreadyInChannel (cref_str sender_nick, cref_str recipient_nick, cref_str channelname) {
	return ":" + server_name + " 443 " + sender_nick + " " + recipient_nick + " " + channelname + " :is already on channel" + crlf();
}

std::string Replies::err_invalidChannelName (cref_str nickname, cref_str invChannelname) {
	return ":" + server_name + " 476 " + nickname + " " + invChannelname + " :Bad Channel Mask" + crlf();
}

std::string Replies::err_recipientNotInChannel (cref_str sender, cref_str recipient, cref_str channelname) {
	return ":" + server_name + " 441 " + sender + " " + recipient + " " + channelname + " :They aren't on that channel" + crlf();
}

std::string Replies::err_keySet (cref_str nickname, cref_str channelname) {
	return ":" + server_name + " 467 " + nickname + " " + channelname + " :Channel key already set" + crlf();
}