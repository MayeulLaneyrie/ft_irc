/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 17:50:45 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/29 13:28:13 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_VOID(Msg & cmd) // ----------------------------------------- VOID
{
	(void)cmd;
	return (0);
}

int User::_cmd_PASS(Msg & cmd) // ----------------------------------------- PASS
{
	if (cmd.getPayload().empty())
		return (rpl(461, "PASS"));

	if (_reg_status == REG_OK)
		return (rpl(462));

	if (_serv->checkPass(cmd.getPayload()))
		_reg_status |= REG_PASS;
	else
		_reg_status |= REG_MISM;
	return (0);
}

int User::_cmd_NICK(Msg & cmd) // ----------------------------------------- NICK
{
	str nick = cmd.getPayload();
	if (nick.empty())
		return (rpl(431));

	if (nick[0] == ':')
		nick.erase(0, 1);

	if (_reg_status & REG_NICK && nick == _nick)
		return (0);

	if (nick.size() > 16 || nick.empty()
			|| nick.find('#') != str::npos
			|| nick.find(':') != str::npos
			|| nick.find(' ') != str::npos)
		return (rpl(432, nick));

	if (_serv->getUserByNick(nick))
		return (rpl(433, nick));

	if (_reg_status & REG_USER && _reg_status & REG_MISM)
		return (error(":Access denied, wrong password"));
	if (_reg_status & REG_USER && !(_reg_status & REG_PASS))
		return (error(":Access denied, password wasn't provided"));

	_serv->unregisterUser(this);
	_nick = nick;
	_serv->registerUser(this);

	if (_reg_status & REG_NICK)
		return (0);
	_reg_status |= REG_NICK;

	if (_reg_status & REG_USER) {
		rpl(1); rpl(2); rpl(3); rpl(4);
	}
	return (0);
}

int User::_cmd_USER(Msg & cmd) // ----------------------------------------- USER
{
	str_vec arg = cmd.payloadAsVector(4);
	if (arg.size() != 4) 
		return (rpl(461, "USER"));

	if (_reg_status == REG_OK)
		return (rpl(462));

	if (_reg_status & REG_NICK && _reg_status & REG_MISM)
		return (error(":Access denied, wrong password"));
	if (_reg_status & REG_NICK && !(_reg_status & REG_PASS))
		return (error(":Access denied, password wasn't provided"));

	_username = arg[0];
	_realname = arg[3];
	_reg_status |= REG_USER;

	if (_reg_status & REG_NICK) {
		rpl(1); rpl(2); rpl(3); rpl(4);
	}
	return (0);
}

int User::_cmd_PING(Msg & cmd) // ----------------------------------------- PING
{
	str_vec arg = cmd.payloadAsVector(1);
	if (!arg.size())
		return (rpl(461, "PING"));

	user_send(
		Msg(this, ":" SERVER_NAME, "PONG", str(SERVER_NAME) + " :" + arg[0])
	);

	return (0);
}

int User::_cmd_QUIT(Msg & cmd) // ----------------------------------------- QUIT
{
	str reason = cmd.getPayload();

	if (reason.empty())
		reason = "Client exited";
	if (reason[0] == ':')
		reason.erase(0, 1);
	error(str(":Okay, bye: (") + _username + "@whatever) [" + reason + ']');
	return (1);
}