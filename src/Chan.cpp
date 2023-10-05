/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chan.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 18:01:09 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chan.hpp"

Chan::Chan(str name) :
	_mode(0),
	_name(name),
	_passwd(""),
	_topic(""),
	_usercount(0),
	_usermax(16)
{}

Chan::Chan(Chan const & src)
{
	*this = src;
}

Chan::~Chan(void)
{}

Chan & Chan::operator=(Chan const & rhs)
{
	this->_name = rhs._name;
	this->_mode = rhs._mode;
	this->_passwd = rhs._passwd;
	this->_topic = rhs._topic;
	this->_usercount = rhs._usercount,
	this->_usermax = rhs._usermax;
	return (*this);
}

int Chan::chan_send(Msg const & msg)
{
	std::map<str, User *>::iterator it = _users.begin();

	for (it = _users.begin(); it != _users.end(); ++it)
		it->second->user_send(msg);
	return (0);
}

void Chan::addUser(User & user)
{
	_users[user.getNick()] = &user;
	_usercount++;
}

void Chan::rmUser(User const & user)
{
	_users.erase(user.getNick());
	_usercount--;
}

User * Chan::getUser(str nick) const
{
	if (_users.count(nick))
		return (_users.at(nick));
	return (NULL);
}

void Chan::renameUser(User * user, str to) {
	_users.erase(user->getNick());
	_users[to] = user;
}

std::map<str, User *>::const_iterator Chan::begin(void) const
{
	return (_users.begin());
}

std::map<str, User *>::const_iterator Chan::end(void) const
{
	return (_users.end());
}

void Chan::addOperator(User *user)
{
	_operators.insert(user);
}

void Chan::rmOperator(User *user)
{
	_operators.erase(user);
}

int Chan::isOperator(User *user)
{
	return (_operators.count(user));
}

void Chan::invite(User *user)
{
	_invited.insert(user);
}

void Chan::uninvite(User *user)
{
	_invited.erase(user);
}

int Chan::isInvited(User *user)
{
	return (_invited.count(user));
}

void Chan::setPasswd(str passwd)
{
	_passwd = passwd;
}

int Chan::checkPasswd(str passwd)
{
	return (passwd == _passwd);
}

unsigned int Chan::checkMode(unsigned int mode)
{
	return (_mode & mode);
}

void Chan::setMode(unsigned int mode, int val)
{
	if (val)
		_mode |= mode;
	else
		_mode &= (~mode);
}

int Chan::isFull(void)
{
	return (_usercount >= _usermax);
}