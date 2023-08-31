/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/08/31 12:49:58 by mlaneyri         ###   ########.fr       */
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

// PRIVATE STUFF ---------------------------------------------------------------

void User::_exec_command(void)
{
	size_t cmd_len = _ibuffer.find("\r\n");

	if (cmd_len == std::string::npos)
		return ;

	std::string command = _ibuffer.substr(0, cmd_len);

	_ibuffer.erase(0, cmd_len + 2);

	std::cout
		<< "User " << _nick << " (#" << _fd
		<< ") executing the following command: [ "
		<< command << " ]" << std::endl;
	
	_ibuffer.clear();
}

// PUBLIC STUFF ----------------------------------------------------------------

int User::do_stuff(void)
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

	if (_ibuffer.find("\r\n") == std::string::npos)
		std::cout << _cbuffer << " ..." << std::endl;

	_exec_command();

	return (len);
}
