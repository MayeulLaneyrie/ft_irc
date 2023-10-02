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
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		return (rpl(461, "PASS"));

	if (_reg_status == REG_OK)
		return (rpl(462));

	if (_serv->checkPass(arg[0]))
		_reg_status |= REG_PASS;
	else
		_reg_status |= REG_MISM;
	return (0);
}

int User::_cmd_NICK(Msg & cmd) // ----------------------------------------- NICK
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		return (rpl(431, "NICK"));

	if (_reg_status & REG_NICK && arg[0] == _nick)
		return (0);

	if (arg[0].size() > 16 || arg[0].empty()
			|| arg[0].find('#') != str::npos
			|| arg[0].find(':') != str::npos
			|| arg[0].find(' ') != str::npos)
		return (rpl(432, arg[0]));

	if (_serv->getUserByNick(arg[0]))
		return (rpl(433, arg[0]));

	if (_reg_status & REG_USER && _reg_status & REG_MISM)
		return (error(":Access denied, wrong password"));
	if (_reg_status & REG_USER && !(_reg_status & REG_PASS))
		return (error(":Access denied, password wasn't provided"));

	_serv->unregisterUser(this);
	_nick = arg[0];
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
	str_vec arg = cmd.payloadAsVector(4, 1);
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
	str_vec arg = cmd.payloadAsVector(1, 1);
	if (arg.empty())
		return (rpl(461, "PING"));

	user_send(
		Msg(this, ":" SERVER_NAME, "PONG", str(SERVER_NAME) + " :" + arg[0])
	);

	return (0);
}

int User::_cmd_QUIT(Msg & cmd) // ----------------------------------------- QUIT
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		arg.push_back("Client exited");

	error(str(":Okay, bye: (") + _username + "@whatever) [" + arg[0] + ']');
	return (1);
}

int User::_cmd_PRIVMSG(Msg & cmd) // ----------------------------------- PRIVMSG
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() != 2)
		return (rpl(461, "PRIVMSG"));
	
	User * target = _serv->getUserByNick(arg[0]);
	if (!target)
		return (rpl(401, arg[0]));

	Msg privmsg(target, str(":") + _nick, "PRIVMSG", arg[0] + " :" + arg[1]);
	privmsg.msg_send();
	return (0);
}