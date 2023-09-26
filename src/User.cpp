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

int	User::_register_cmd(Msg cmd, int cmd_id)
{
	switch (cmd_id) {
		case (CMD_PASS): // ----------------------------------------------------
			if (cmd.getPayload().empty())
				rpl(461, "PASS");
			else if (_reg_status == REG_OK)
				rpl(462);
			else if (_serv->checkPass(cmd.getPayload()))
				_reg_status |= REG_PASS;
			else
				_reg_status |= REG_MISM;
			break ;
		case (CMD_NICK): // ----------------------------------------------------
			if (cmd.getPayload().empty())
				rpl(461, "NICK");
			else if (_reg_status & REG_USER && !(_reg_status & REG_PASS)) {
				user_send(Msg(this, "", "ERROR", ":Access denied"));
				return (1);
			}
			else if (_serv->getUserByNick(cmd.getPayload()))
				rpl(433, cmd.getPayload());
			else {
				_nick = cmd.getPayload(); // TODO: CHECK IF THE NICK IS ACTUALLY VALID !!!
				_serv->setAsRegisterd(this);
				_reg_status |= REG_NICK;
				if (_reg_status & REG_USER) {
					rpl(1); rpl(2); rpl(3); rpl(4);
				}
			}
			break ;
		case (CMD_USER): // ----------------------------------------------------
			str_vec arg = cmd.payloadAsVector(4);
			if (arg.size() != 4) 
				rpl(461, "USER");
			else if (_reg_status == REG_OK)
				rpl(462);
			else {
				_reg_status |= REG_USER;
			}
			break ;
	}
	return (0);
}

int User::_exec_command(void)
{
	// TODO: the following will have to go static some way or another
	std::map<str, int> cmd_map;

	cmd_map[""] = CMD_NULL;
	cmd_map["PASS"] = CMD_PASS;
	cmd_map["NICK"] = CMD_NICK;
	cmd_map["USER"] = CMD_USER;
	cmd_map["PING"] = CMD_PING;

	// Extraction of the first message of the string
	size_t msg_len = _ibuffer.find("\r\n");
	str msg_str = _ibuffer.substr(0, msg_len);
	_ibuffer.erase(0, msg_len + 2);

	std::cout << "\e[1m" << getNick() << " ->\e[0m " << msg_str << std::endl;

	Msg cmd(this, msg_str);
	int cmd_id = cmd_map[cmd.getCmd()];
	if (cmd_id <= CMD_USER)
		return (_register_cmd(cmd, cmd_id));
	if (_reg_status != REG_OK) {
		rpl(451);
		return (0);
	}	
	switch (cmd_id) {
		case (CMD_PING): // ----------------------------------------------------
			str_vec arg = cmd.payloadAsVector(1);
			if (!arg.size())
				rpl(461, "PING");
			else
				user_send(Msg(this, ":"SERVER_NAME, "PONG", str(":") + arg[0]));
			break ;
	}
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

	return (rpl.msg_send());	
}

int User::user_recv(void)
{
	int len = recv(_fd, _cbuffer, RECV_BUFF_SIZE - 1, 0);
	
	if (!len) {
		std::cout << "User " << getNick() << " disconnected from the server." << std::endl;
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