/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/12 17:30:16 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// COPLIEN =====================================================================

User::User(Serv * serv, int fd) :
	_fd(fd),
	_nick(""),
	_ibuffer(""),
	_reg_status(0),
	_serv(serv)
{}

User::User(User const & src)
{
	*this = src;
}

User::~User(void)
{
	close(_fd);
}

User & User::operator=(User const & rhs)
{
	close(_fd);
	_fd = rhs._fd;
	_nick = rhs._nick;
	_ibuffer = rhs._ibuffer;
	_reg_status = rhs._reg_status;
	return (*this);
}

// INTERNAL STUFF ==============================================================

int User::_exec_command(void)
{
	// TODO: the following will have to go static some way or another
	std::map<str, ft_cmd> cmd_map;

	cmd_map[""]		= &User::_cmd_VOID;
	cmd_map["PASS"] = &User::_cmd_PASS;
	cmd_map["NICK"] = &User::_cmd_NICK;
	cmd_map["USER"] = &User::_cmd_USER;
	cmd_map["PING"] = &User::_cmd_PING;

	// Extraction of the first message of the string
	size_t msg_len = _ibuffer.find("\r\n");
	str msg_str = _ibuffer.substr(0, msg_len);
	_ibuffer.erase(0, msg_len + 2);

	std::cout << "\e[1m" << getNick() << " ->\e[0m " << msg_str << std::endl;

	Msg cmd(this, msg_str);

	if (_reg_status != REG_OK
			&& cmd.getCmd() != "PASS"
			&& cmd.getCmd() != "NICK"
			&& cmd.getCmd() != "USER")
			return (rpl(451));

	return ((this->*cmd_map[cmd.getCmd()])(cmd));
}

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
	if (cmd.getPayload().empty())
		return (rpl(461, "NICK"));

	if (_reg_status & REG_USER && _reg_status & REG_MISM)
		return (error(":Access denied, wrong password"));
	if (_reg_status & REG_USER && !(_reg_status & REG_PASS))
		return (error(":Access denied, password wasn't provided"));

	if (_serv->getUserByNick(cmd.getPayload()))
		return (rpl(433, cmd.getPayload()));

	_nick = cmd.getPayload(); // TODO: CHECK IF THE NICK IS ACTUALLY VALID !!!
	_serv->setAsRegisterd(this);
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

	user_send(Msg(this, ":" SERVER_NAME, "PONG", str(":") + arg[0]));

	return (0);
}

// ACCESSORS ===================================================================

str User::getNick(void) const
{
	if (!_nick.empty())
		return (_nick);
	std::ostringstream oss("");
	oss << "FD#" << _fd;
	return (oss.str());
}

str User::getUsername(void) const { return (_username); };

Serv * User::getServ(void) const { return (_serv); }

// OTHER PUBLIC MEMBER FUNCTIONS ===============================================

int User::rpl(int num, str const & p1, str const & p2)
{
	Msg rpl(num, this, p1, p2);

	rpl.msg_send();
	return (0);
}

int User::error(str const & msg)
{
	user_send(Msg(this, "", "ERROR", msg));
	return (1);
}

int User::user_recv(void)
{
	int len = recv(_fd, _cbuffer, RECV_BUFF_SIZE - 1, 0);
	
	if (!len) {
		std::cout
			<< "\e[1mUser " << getNick() << " disconnected from the server.\e[0m"
			<< std::endl;
		return (0);
	}

	_cbuffer[len] = '\0';
	_ibuffer.append(_cbuffer);

	while (_ibuffer.find("\r\n") != str::npos)
		if (_exec_command())
			return (0);

	return (len);
}

int User::user_send(Msg const & msg) const
{
	std::cout << "\e[1m" << getNick() << " <-\e[0m " << msg.getStr();
	return (send(_fd, msg.getStr().c_str(), msg.getStr().size(), 0));
}