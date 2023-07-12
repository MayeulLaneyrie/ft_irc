/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 17:31:14 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Server.hpp"

Server::Server(void) :
	_port(6667),
	_password(""),
	_max_client_id(0),
	_max_chan_id(0)
{}

Server::Server(int port, std::string password) :
	_password(password),
	_max_client_id(0),
	_max_chan_id(0)
{
	if (port < 1 || port > 65535)
		die("sdsd", __FILE__, __LINE__, "Invalid port value", port);
	this->_port = port;
}

Server::Server(Server const & src)
{
	*this = src;
}

Server::~Server(void)
{}

Server & Server::operator=(Server const & rhs) {
	this->_port = rhs._port;
	this->_password = rhs._password;
	this->_max_client_id = rhs._max_client_id;
	this->_max_chan_id = rhs._max_chan_id;
	this->_chans = rhs._chans;
	this->_clients = rhs._clients;
	return (*this);
}

int Server::run(void)
{
	std::cout << "Server started!" << std::endl;
	return (0);
}

/*
 * TODO for these 2 functs: check beforehand whether the client already exists ?
 */
Client & Server::getClient(int id)
{
	return (this->_clients[id]);
}

Chan & Server::getChan(int id)
{
	return (this->_chans[id]);
}
