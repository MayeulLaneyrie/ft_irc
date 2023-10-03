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
	_topic("")
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
	return (*this);
}

int Chan::chan_send(Msg const & msg)
{
	std::map<str, User *>::iterator it = _users.begin();

	for (it = _users.begin(); it != _users.end(); ++it)
		it->second->user_send(msg);
	return (0);
}

int Chan::addUser(User & user)
{
	_users[user.getNick()] = &user;
	return (0);
}

int Chan::rmUser(User const & user)
{
	_users.erase(user.getNick());
	return (0);
}

User * Chan::getUser(str nick) const
{
	if (_users.count(nick))
		return (_users.at(nick));
	return (NULL);
}