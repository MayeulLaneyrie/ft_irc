/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/08/30 17:01:35 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// COPLIEN ---------------------------------------------------------------------

User::User(int fd) :
	_fd(fd),
	_nick(""),
	_ibuffer(""),
	_register_status(0);
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

int User::_is_command_complete(void)
{
	return (_ibuffer.size() > 1
			&& _ibuffer[_ibuffer.size() - 2] == '\r'
			&& _ibuffer[_ibuffer.size() - 1] == '\n');
}

void User::_exec_command(void)
{
	_ibuffer.erase(_ibuffer.size() - 2, 2);
	
	std::cout
		<< "User " << _nick << " (#" << _fd
		<< ") executing the following command: [ "
		<< _ibuffer << " ]" << std::endl;
	
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
	
	if (!_is_command_complete()) {
		std::cout << _cbuffer << " ..." << std::endl;
		return (len);
	}

	_exec_command();

	return (len);
}
