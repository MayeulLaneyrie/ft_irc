/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:15:21 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:15:25 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_NICK(Msg & cmd) // ----------------------------------------- NICK
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		return (rpl(ERR_NONICKNAMEGIVEN));

	if (_reg_status & REG_NICK && arg[0] == _nick)
		return (0);

	if (!is_valid_name(arg[0]) || is_name_chan(arg[0]))
		return (rpl(ERR_ERRONEUSNICKNAME, arg[0]));

	if (_serv->getUser(arg[0]))
		return (rpl(ERR_NICKNAMEINUSE, arg[0]));

	if (_reg_status & REG_USER && _reg_status & REG_MISM)
		return (error(":Access denied, wrong password"));
	if (_reg_status & REG_USER && !(_reg_status & REG_PASS))
		return (error(":Access denied, password wasn't provided"));

	std::cout << C_CYAN << getNick() << " --> " << arg[0] << C_R << std::endl;

	_serv->renameUser(this, arg[0]);
	std::map<str, Chan *>::iterator it;
	for (it = _chans.begin(); it != _chans.end(); ++it)
		it->second->renameUser(this, arg[0]);

	broadcast(Msg(_pref, "NICK", arg[0]));
	if (_reg_status == REG_OK)
		user_send(Msg(_pref, "NICK", arg[0]));

	_nick = arg[0];
	_pref = _nick + '!' + _username + "@whatever";

	if (_reg_status & REG_NICK)
		return (0);
	_reg_status |= REG_NICK;

	if (_reg_status & REG_USER) {
		rpl(RPL_WELCOME);
		rpl(RPL_YOURHOST);
		rpl(RPL_CREATED);
		rpl(RPL_MYINFO);
	}
	return (0);
}