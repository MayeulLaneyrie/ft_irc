/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 19:42:04 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Serv.hpp"

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

Serv::Serv(void) :
	_port(6667),
	_password(""),
	_max_user_id(0),
	_max_chan_id(0)
{}

Serv::Serv(int port, std::string password) :
	_password(password),
	_max_user_id(0),
	_max_chan_id(0)
{
	if (port < 1 || port > 65535)
		die("sdsd", __FILE__, __LINE__, "Invalid port value", port);
	this->_port = port;
}

Serv::Serv(Serv const & src)
{
	*this = src;
}

Serv::~Serv(void)
{}

Serv & Serv::operator=(Serv const & rhs) {
	this->_port = rhs._port;
	this->_socket = rhs._socket;
	this->_password = rhs._password;
	this->_max_user_id = rhs._max_user_id;
	this->_max_chan_id = rhs._max_chan_id;
	this->_chans = rhs._chans;
	this->_users = rhs._users;
	return (*this);
}

// INTERNAL STUFF --------------------------------------------------------------

int Serv::setup_socket(void)
{

	return (0);
}

// ACCESSORS -------------------------------------------------------------------

User & Serv::getUser(int id)
{
	if (!this->_users.count(id)) // This isn't supposed to happen.
		die("sds", __FILE__, __LINE__, "User doesn't exist");
	return (this->_users[id]);
}

Chan & Serv::getChan(int id)
{
	if (!this->_users.count(id)) // Same.
		die("sds", __FILE__, __LINE__, "Chan doesn't exist");
	return (this->_chans[id]);
}

int Serv::run(void)
{
	std::cout << "Server launched!" << std::endl;
	return (0);
}


