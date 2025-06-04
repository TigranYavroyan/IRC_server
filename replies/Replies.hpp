#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

class Replies {
	static std::string server_name;
public:
	static std::string crlf ();
	typedef const std::string& cref_str;

	static std::string connected (cref_str nickname);
	static std::string umodeIs (cref_str hostname, cref_str channelname, cref_str mode, cref_str user);
	static std::string creationOnTime (cref_str nickname, cref_str channelname, cref_str creationtime);
	static std::string channelModes (cref_str nickname, cref_str channelname, cref_str modes);
	static std::string changeMode (cref_str hostname, cref_str channelname, cref_str mode, cref_str arguments);
	static std::string nickChange (cref_str oldnickname, cref_str nickname);
	static std::string joinMsg (cref_str hostname, cref_str ipaddress, cref_str channelname);
	static std::string namReply (cref_str nickname, cref_str channelname, cref_str clientslist);
	static std::string endOfNames (cref_str nickname, cref_str channelname);
	static std::string topicIs (cref_str nickname, cref_str channelname, cref_str topic);

	// ///////// ERRORS ////////////////
	static std::string err_needModeParm(cref_str channelname, cref_str mode);
	static std::string err_invaliDModeParm(cref_str channelname, cref_str mode);
	static std::string err_keySet(cref_str channelname);
	static std::string err_unknownMode(cref_str nickname, cref_str channelname, cref_str mode);
	static std::string err_notEnoughParam(cref_str command_name ,cref_str nickname);
	static std::string err_channelNotFound(cref_str nickname, cref_str channelname);
	static std::string err_notopeRATOR(cref_str channelname);
	static std::string err_noSuchNick(cref_str channelname, cref_str name);
	static std::string err_incorpass(cref_str nickname);
	static std::string err_alreadyRegistered(cref_str nickname);
	static std::string err_noNickName(cref_str nickname);
	static std::string err_nickInUse(cref_str nickname);
	static std::string err_oneUsNick(cref_str nickname);
	static std::string err_notRegistered(cref_str nickname);
	static std::string err_cmdnotFound(cref_str nickname, cref_str command);
	static std::string err_cannotJoin(cref_str nickname, cref_str channelname);

};

#endif // REPLIES_HPP