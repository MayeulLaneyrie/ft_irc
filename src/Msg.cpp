/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/13 12:35:21 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Msg.hpp"

Msg::Msg(void)
{}

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

	ret[1] = RPL_WELCOME;
	ret[2] = RPL_YOURHOST;
	ret[3] = RPL_CREATED;
	ret[4] = RPL_MYINFO;

	ret[433] = ERR_NICKNAMEINUSE;
	ret[451] = ERR_NOTREGISTERED;
	ret[461] = ERR_NEEDMOREPARAMS;
	ret[462] = ERR_ALREADYREGISTERED;

	return (ret);
}

Msg::Msg(int num, User * contact, str const & p1, str const & p2)
{

	static std::map<int, str> rpl_map = _gen_rpl_map();

	std::map<str, str> vars;

	str rpl_string(contact->getNick());
	rpl_string += ' ';
	rpl_string += rpl_map[num];

	vars["{n}"] = contact->getNick();
	vars["{u}"] = contact->getUsername();
	vars["{h}"] = "host";
	vars["{nw}"] = "network";
	vars["{sn}"] = SERVER_NAME;
	vars["{dt}"] = contact->getServ()->getDatetime();
	vars["{v}"] = "0.0";
	vars["{aum}"] = "o";
	vars["{acm}"] = "itkol";
	vars["{1}"] = p1;
	vars["{2}"] = p2;

	std::map<str const, str>::const_iterator it;

	for (it = vars.begin(); it != vars.end(); ++it)
		sed(rpl_string, it->first, it->second);

	std::ostringstream rpl_cmd("");

	rpl_cmd << std::setw(3) << std::setfill('0') << num;
	
	_contact = contact;
	_prefix = ":" SERVER_NAME;
	_cmd = rpl_cmd.str();
	_payload = rpl_string;

	_regen_str();
}

Msg::Msg(Msg const & src)
{
	*this = src;
}

Msg::~Msg()
{}

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
		oss << _prefix << ' ' << _cmd << ' ' << _payload << "\r\n";
	_as_str = oss.str();
}

// ACCESSORS -------------------------------------------------------------------

str Msg::getPrefix(void) const { return (_prefix); }
str Msg::getCmd(void) const { return (_cmd); }
str Msg::getPayload(void) const { return (_payload); }

User * Msg::getContact(void) const { return (_contact); }

str Msg::getStr(void) const { return (_as_str); }

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

str_vec Msg::payloadAsVector(int ac) const
{
	str_vec ret;
	str tmp = _payload;

	for (int i = 1; i < ac && tmp.size(); ++i)
		ret.push_back(extract_first_word(tmp));
	if (tmp.size() && tmp[0] == ':')
		ret.push_back(tmp.erase(0, 1));
	else if (tmp.size())
		ret.push_back(extract_first_word(tmp));
	return (ret);
}

int Msg::msg_send(void) const
{
	return _contact->user_send(*this);
}