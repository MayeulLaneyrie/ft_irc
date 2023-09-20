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

Msg::Msg(User * contact, std::string const & pref, std::string const & cmd, std::string const & payld) :
	_prefix(pref),
	_cmd(cmd),
	_payload(payld),
	_contact(contact)
{}

Msg::Msg(User * contact, std::string const & s)
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
}

std::map<int, std::string> gen_rpl_map(void)
{
	std::map<int, std::string> ret;

	ret[1] = RPL_WELCOME;
	ret[2] = RPL_YOURHOST;
	ret[3] = RPL_CREATED;
	ret[4] = RPL_MYINFO;

	ret[461] = ERR_NEEDMOREPARAMS;
	ret[462] = ERR_ALREADYREGISTERED;

	return (ret);
}

Msg::Msg(int num, User * contact, std::string const & p1, std::string const & p2)
{
	static std::map<int, std::string> rpl_map = gen_rpl_map();
	std::map<std::string, std::string> vars;

	std::string rpl_string(contact->getNick());
	rpl_string += ' ';
	rpl_string += rpl_map[num];

	vars["{n}"] = contact->getNick();
	vars["{u}"] = "user";
	vars["{h}"] = "host";
	vars["{nw}"] = "network";
	vars["{sn}"] = SERVER_NAME;
	vars["{dt}"] = "datetime";
	vars["{v}"] = "0.42";
	vars["{aum}"] = ".";
	vars["{acm}"] = "itkol";
	vars["{1}"] = p1;
	vars["{2}"] = p2;

	std::map<std::string const, std::string>::const_iterator it;

	for (it = vars.begin(); it != vars.end(); ++it)
		sed(rpl_string, it->first, it->second);

	std::ostringstream rpl_cmd("");

	rpl_cmd << std::setw(3) << std::setfill('0') << num;
	
	_contact = contact;
	_prefix = ":"SERVER_NAME;
	_cmd = rpl_cmd.str();
	_payload = rpl_string;
}

Msg::~Msg()
{}

// ACCESSORS -------------------------------------------------------------------

std::string Msg::getPrefix(void) const { return (_prefix); }
std::string Msg::getCmd(void) const { return (_cmd); }
std::string Msg::getPayload(void) const { return (_payload); }

User * Msg::getContact(void) const { return (_contact); }

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

std::string Msg::str(void)
{
	std::ostringstream oss("");

	oss << _prefix << ' ' << _cmd << ' ' << _payload << "\r\n";
	return (oss.str());
}

std::vector<std::string> Msg::payloadAsVector(void) const
{
	std::vector<std::string> ret;

	return (ret);
}

int Msg::msg_send(void)
{
	return _contact->user_send(str());
}