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

// COPLIEN ---------------------------------------------------------------------

User::User(int fd) :
	_fd(fd),
	_nick(""),
	_ibuffer(""),
	_register_status(0)
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
	_register_status = rhs._register_status;
	return (*this);
}

// INTERNAL STUFF ---------------------------------------------------------------

void User::_exec_command(void)
{
	// TODO: the following has to go static some way or another
	std::map<std::string, int> cmd_map;

	cmd_map["PASS"] = CMD_PASS;
	cmd_map["NICK"] = CMD_NICK;
	cmd_map["USER"] = CMD_USER;
	cmd_map["PING"] = CMD_PING;

	size_t cmd_len = _ibuffer.find("\r\n");
	std::string command = _ibuffer.substr(0, cmd_len);
	_ibuffer.erase(0, cmd_len + 2);

	std::cout << ':' << _nick << " (#" << _fd << "): [ " << command << " ]" << std::endl;

	std::string cmd_str = extract_cmd(command);
	switch (cmd_map[cmd_str]) {
		case (CMD_USER):
			rpl(1);
			rpl(2);
			rpl(3);
			rpl(4);
			break;
		case (CMD_PING):
			user_send("PONG ircserv\r\n");
			break;
	}
}

// ACCESSORS -------------------------------------------------------------------

std::string User::getNick(void) const { return (_nick); }

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

int User::rpl(int num, std::string const & p1, std::string const & p2)
{
	Msg rpl(num, this, p1, p2);

	return (rpl.msg_send());	
}

int User::user_recv(void)
{
	int len = recv(_fd, _cbuffer, RECV_BUFF_SIZE - 1, 0);
	
	if (!len) {
		std::cout
			<< "User " << _nick << " (#" << _fd
			<< ") disconnected from the server"
			<< std::endl;
		return (0);
	}

	_cbuffer[len] = '\0';
	_ibuffer.append(_cbuffer);

	while (_ibuffer.find("\r\n") != std::string::npos)
		_exec_command();

	return (len);
}

int User::user_send(std::string const & s) const
{
	std::cout << "To: " << _nick << " (#" << _fd << "): " << s;
	return (send(_fd, s.c_str(), s.size(), 0));
}
