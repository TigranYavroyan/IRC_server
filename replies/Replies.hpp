#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>
#include <User.hpp>

class Replies {
	static std::string server_name;
public:
	static std::string crlf ();
	typedef const std::string& cref_str;

	static std::string cap_ls ();
	static std::string privateMessage(const User& sender, cref_str target, cref_str message);
	static std::string connected (cref_str nickname);
	static std::string umodeIs (cref_str hostname, cref_str channelname, cref_str mode, cref_str user);
	static std::string creationOnTime (cref_str nickname, cref_str channelname, cref_str creationtime);
	static std::string channelModes (cref_str nickname, cref_str channelname, cref_str modes);
	static std::string changeMode (cref_str hostname, cref_str channelname, cref_str mode, cref_str arguments);
	static std::string nickChange (cref_str oldnickname, cref_str nickname);
	static std::string joinMsg (const User& joined_user, cref_str channelname);
	static std::string quitMsg (const User& quited_user, cref_str quit_msg = "Client Quit");
	static std::string namReply (cref_str nickname, cref_str channelname, cref_str clientslist);
	static std::string endOfNames (cref_str nickname, cref_str channelname);
	static std::string topicIs (cref_str nickname, cref_str channelname, cref_str topic);
	static std::string userFullName(const User& user);
	static std::string partMsg (const User& quited_user, cref_str channelname ,cref_str part_msg);
	

	// ///////// ERRORS ////////////////
	static std::string err_needModeParm(cref_str command_name, cref_str channelname, cref_str mode);
	static std::string err_invaliDModeParm(cref_str nickname, char mode);
	static std::string err_keySet(cref_str command_name, cref_str channelname);
	static std::string err_unknownMode(cref_str command_name, cref_str nickname, cref_str channelname, cref_str mode);
	static std::string err_notEnoughParam(cref_str command_name, cref_str nickname);
	static std::string err_notOperator(cref_str channelname);
	static std::string err_noSuchNick(cref_str command_name, cref_str recipient);
	static std::string err_incorpass(cref_str command_name, cref_str nickname);
	static std::string err_alreadyRegistered(cref_str command_name, cref_str nickname);
	static std::string err_noNickName(cref_str command_name, cref_str nickname);
	static std::string err_nickInUse(cref_str command_name, cref_str nickname);
	static std::string err_oneUsNick(cref_str command_name, cref_str nickname);
	static std::string err_notRegistered(cref_str command_name, cref_str nickname);
	static std::string err_cmdnotFound(cref_str command_name, cref_str nickname);
	static std::string err_cannotJoin(cref_str command_name, cref_str nickname, cref_str channelname);
	static std::string err_noTextToSend(cref_str nickname);
	static std::string err_noRecipientGiven(cref_str nickname, cref_str command_name);
	static std::string err_noOnThatChannel(cref_str nickname, cref_str channelname);
	static std::string err_noSuchChannel (cref_str nickname, cref_str channelname);

};

#endif // REPLIES_HPP