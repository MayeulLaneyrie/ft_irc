/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/29 12:56:29 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Msg.hpp"

Msg::Msg(User * contact, str const & pref, str const & cmd, str const & payld) :
	_prefix(pref),
	_cmd(cmd),
	_payload(payld),
	_contact(contact)
{
	_regen_str();
}

Msg::Msg(User * contact, str const & s)
{
	_payload = s;
	_contact = contact;
	if (_payload.empty())
		return ;
	if (_payload[0] == ':') {
		_prefix = extract_first_word(_payload);
		_prefix.erase(0, 1);
	}
	_cmd = extract_first_word(_payload);
	_regen_str();
}

std::map<int, str> Msg::_gen_rpl_map(void)
{
	std::map<int, str> ret;

	ret[RPL_WELCOME] = ":Welcome to the 42paris.fr IRC Network, {n}[!{u}@whatever]";
	ret[RPL_YOURHOST] = ":Your host is {sn}, running version {v}";
	ret[RPL_CREATED] = ":This server was created {dt}";
	ret[RPL_MYINFO] = "{sn} {v} o itkol";
	ret[RPL_YOUREOPER] = ":You are now an IRC operator";

	ret[ERR_NOSUCHNICK] = "{1} :No such nick/channel";
	ret[ERR_NOSUCHCHANNEL] = "{1} ::No such channel";
	ret[ERR_UNKNOWNCOMMAND] = "{1} :Unknown command";
	ret[ERR_NONICKNAMEGIVEN] = ":No nickname given";
	ret[ERR_ERRONEUSNICKNAME] = "{1} :Erroneus nickname";
	ret[ERR_NICKNAMEINUSE] = "{1} :Nickname is already in use";
	ret[ERR_NOTONCHANNEL] = "{1} :You're not on that channel";
	ret[ERR_USERONCHANNEL] = "{1} :is already on channel";
	ret[ERR_NOTREGISTERED] = ":You have not registered";
	ret[ERR_NEEDMOREPARAMS] = "{1} :Not enough parameters";
	ret[ERR_ALREADYREGISTERED] = ":You may not reregister";
	ret[ERR_PASSWDMISMATCH] = ":Password incorrect";
	ret[ERR_CHANNELISFULL] = "{1} :Cannot join channel (+l)";
	ret[ERR_INVITEONLYCHAN] = "{1} :Cannot join channel (+i)";
	ret[ERR_BADCHANNELKEY] = "{1} :Cannot join channel (+k)";
	ret[ERR_NOPRIVILEGES] = ":Permission Denied- You're not an IRC operator";
	ret[ERR_CHANOPRIVSNEEDED] = "{1} :You're not channel operator";
	ret[ERR_NOOPERHOST] = ":Invalid oper credentials";

	return (ret);
}

Msg::Msg(int num, User * contact, str const & p1)
{

	static const std::map<int, str> rpl_map = _gen_rpl_map();

	std::map<str, str> vars;
	str rpl_string = contact->getNick() + ' ';

	if (!rpl_map.count(num))
		rpl_string += ":This reply hasn't been implemented yet :/";
	else
		rpl_string += rpl_map.at(num);

	vars["{n}"] = contact->getNick();
	vars["{u}"] = contact->getUsername();
	vars["{sn}"] = SERVER_NAME;
	vars["{dt}"] = contact->getServ()->getDatetime();
	vars["{v}"] = "0.42";
	vars["{1}"] = p1;

	std::map<str, str>::const_iterator it;
	for (it = vars.begin(); it != vars.end(); ++it)
		sed(rpl_string, it->first, it->second);

	std::ostringstream rpl_cmd("");
	rpl_cmd << std::setw(3) << std::setfill('0') << num;
	_cmd = rpl_cmd.str();

	_contact = contact;
	_prefix = SERVER_NAME;
	_payload = rpl_string;

	_regen_str();
}

Msg::Msg(Msg const & src) {
	*this = src;
}

Msg::~Msg() {}

Msg & Msg::operator=(Msg const & rhs)
{
	_prefix = rhs._prefix;
	_cmd = rhs._cmd;
	_payload = rhs._payload;
	_as_str = rhs._as_str;
	_contact = rhs._contact;
	return (*this);
}


// INTERNAL STUFF --------------------------------------------------------------

void Msg::_regen_str(void)
{
	std::ostringstream oss("");

	if (_prefix.empty())
		oss << _cmd << ' ' << _payload << "\r\n";
	else
		oss << ':' << _prefix << ' ' << _cmd << ' ' << _payload << "\r\n";
	_as_str = oss.str();
}

// ACCESSORS -------------------------------------------------------------------

str Msg::getPrefix(void) const {
	return (_prefix);
}

str Msg::getCmd(void) const {
	return (_cmd);
}

str Msg::getPayload(void) const {
	return (_payload);
}

User * Msg::getContact(void) const {
	return (_contact);
}

str Msg::getStr(void) const {
	return (_as_str);
}

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

str_vec Msg::payloadAsVector(int ac, int requireColon) const
{
	str_vec ret;
	str tmp = _payload;

	for (int i = 1; i < ac && tmp.size(); ++i)
		ret.push_back(extract_first_word(tmp));

	if (!tmp.size())
		return (ret);

	if (tmp[0] == ':')
		ret.push_back(tmp.erase(0, 1));
	else if (!requireColon)
		ret.push_back(tmp);
	else
		ret.push_back(extract_first_word(tmp));

	return (ret);
}

int Msg::msg_send(void) const {
	return (_contact->user_send(*this));
}