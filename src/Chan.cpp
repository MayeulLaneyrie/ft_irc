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

Chan::Chan(Serv * serv, str const & name) :
	_mode(0),
	_name(name),
	_passwd(""),
	_topic(""),
	_usercount(0),
	_usermax(16),
	_serv(serv)
{
	std::cout << C_CYAN << name << " created" C_R << std::endl;
}

Chan::Chan(Chan const & src) {
	*this = src;
}

Chan::~Chan(void) {
	std::cout << C_MAGENTA << _name << " removed" C_R << std::endl;
}

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

int Chan::chan_send(User * source, Msg const & msg)
{
	std::map<str, User *>::iterator it = _users.begin();

	for (it = _users.begin(); it != _users.end(); ++it)
		if (it->second != source)
			it->second->user_send(msg);
	return (0);
}

void Chan::addUser(User * user)
{
	_users[user->getNick()] = user;
	_usercount++;
}

void Chan::rmUser(User * user)
{
	if (!_usercount)
		return ;
	_users.erase(user->getNick());
	_operators.erase(user);
	_invited.erase(user);
	_usercount--;
	if (!_usercount)
		_serv->rmChan(_name);
}

User * Chan::getUser(str const & nick) const
{
	if (_users.count(nick))
		return (_users.at(nick));
	return (NULL);
}

void Chan::renameUser(User * user, str const & to)
{
	_users.erase(user->getNick());
	_users[to] = user;
}

std::map<str, User *>::const_iterator Chan::begin(void) const {
	return (_users.begin());
}

std::map<str, User *>::const_iterator Chan::end(void) const {
	return (_users.end());
}

void Chan::addOperator(User * user) {
	_operators.insert(user);
}

void Chan::rmOperator(User * user) {
	_operators.erase(user);
}

int Chan::isOperator(User * user) const {
	return (_operators.count(user));
}

void Chan::invite(User * user) {
	_invited.insert(user);
}

void Chan::uninvite(User * user) {
	_invited.erase(user);
}

int Chan::isInvited(User * user) const {
	return (_invited.count(user));
}

void Chan::setPasswd(str const & passwd) {
	_passwd = passwd;
}

int Chan::checkPasswd(str const & passwd) const {
	return (passwd == _passwd);
}

str Chan::getPasswd( void ) const {
	return (_passwd);
}

unsigned int Chan::checkMode(unsigned int mode) const {
	return (_mode & mode);
}

void Chan::setMode(unsigned int mode, int val)
{
	if (val)
		_mode |= mode;
	else
		_mode &= (~mode);
}

unsigned int Chan::getMode( void ) const {
	return (_mode);
}

void Chan::setTopic(str topic) {
	_topic = topic;
}
str Chan::getTopic(void) const {
	return (_topic);
}

void Chan::setLimit(int limit) {
	_usermax = limit;
}

int Chan::getLimit( void ) const {
	return (_usermax);
}

int Chan::isFull(void) const {
	return (_usercount >= _usermax);
}