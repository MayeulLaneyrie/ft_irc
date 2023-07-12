/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 18:47:24 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(void) :
	_fd(-1),
	_id(-1),
	_name(""),
	_nick(""),
	_ibuffer("")
{}

User::User(int id, int fd) :
	_fd(fd),
	_id(id),
	_name(""),
	_nick(""),
	_ibuffer("")
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
	this->_fd = rhs._fd;
	this->_id = rhs._id;
	this->_name = rhs._name;
	this->_nick = rhs._nick;
	this->_ibuffer = rhs._ibuffer;
	return (*this);
}


