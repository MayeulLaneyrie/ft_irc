/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/10 17:11:30 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// COPLIEN =====================================================================

User::User(Serv * serv, int fd) :
	_fd(fd),
	_nick(""),
	_ibuffer(""),
	_reg_status(0),
	_is_op(0),
	_stop(0),
	_serv(serv)
{}

User::User(User const & src) {
	*this = src;
}

User::~User( void )
{
	std::map<str, Chan *>::iterator it;
	for (it = _chans.begin(); it != _chans.end(); ++it)
		it->second->rmUser(this);
	close(_fd);
}

User & User::operator=(User const & rhs)
{
	close(_fd);
	_fd = rhs._fd;
	_nick = rhs._nick;
	_username = rhs._username;
	_realname = rhs._realname;
	_ibuffer = rhs._ibuffer;
	_obuffer = rhs._obuffer;
	_reg_status = rhs._reg_status;
	_is_op = rhs._is_op;
	_stop = rhs._stop;
	_serv = rhs._serv;
	return (*this);
}

// INTERNAL STUFF ==============================================================

std::map<str, User::ft_cmd> User::_gen_cmd_map( void )
{
	std::map<str, ft_cmd> ret;

	ret[""]		= &User::_cmd_VOID;
	ret["CAP"]	= &User::_cmd_VOID;
	ret["PONG"] = &User::_cmd_VOID;
	ret["PASS"] = &User::_cmd_PASS;
	ret["NICK"] = &User::_cmd_NICK;
	ret["USER"] = &User::_cmd_USER;
	ret["PING"] = &User::_cmd_PING;
	ret["QUIT"] = &User::_cmd_QUIT;
	ret["PRIVMSG"] = &User::_cmd_PRIVMSG;
	ret["NOTICE"] = &User::_cmd_NOTICE;
	ret["OPER"] = &User::_cmd_OPER;
	ret["KILL"] = &User::_cmd_KILL;
	ret["JOIN"] = &User::_cmd_JOIN;
	ret["TOPIC"] = &User::_cmd_TOPIC;
	ret["KICK"] = &User::_cmd_KICK;
	ret["WHO"] = &User::_cmd_WHO;
	ret["WHOIS"] = &User::_cmd_WHOIS;
	ret["MODE"] = &User::_cmd_MODE;
	ret["PART"] = &User::_cmd_PART;
	ret["INVITE"] = &User::_cmd_INVITE;
	ret["NAMES"] = &User::_cmd_NAMES;
	return (ret);
}

const std::map<str, User::ft_cmd> User::_cmd_map = User::_gen_cmd_map();

std::set<str> User::_gen_prereg_set( void )
{
	std::set<str> ret;

	ret.insert("");
	ret.insert("CAP");
	ret.insert("PASS");
	ret.insert("NICK");
	ret.insert("USER");
	ret.insert("QUIT");

	return (ret);
}

const std::set<str> User::_prereg_set = User::_gen_prereg_set();

int User::_exec_cmd( void )
{
	size_t msg_len = _ibuffer.find("\r\n");
	str msg_str = _ibuffer.substr(0, msg_len);
	_ibuffer.erase(0, msg_len + 2);

	Msg cmd_msg(msg_str);
	str cmd = cmd_msg.getCmd();

	OUT << C_GREEN << getNick() << " >\e[0;32m { " << msg_str << " }" C_R << NL;

	if (_reg_status != REG_OK && !_prereg_set.count(cmd))
		return (rpl(ERR_NOTREGISTERED));

	if (!_cmd_map.count(cmd))
		return (rpl(ERR_UNKNOWNCOMMAND, cmd));

	return (this->*_cmd_map.at(cmd))(cmd_msg);
}

// ACCESSORS ===================================================================

str User::getNick( void ) const
{
	if (!_nick.empty())
		return (_nick);
	std::ostringstream oss("");
	oss << "TMP." << _fd;
	return (oss.str());
}

str User::getUsername( void ) const {
	return (_username);
}

str User::getRealname( void ) const {
	return (_realname);
}

std::map<str, Chan *> User::getChan( void ) const {
	return (_chans);
}

int User::getFd( void ) const {
	return (_fd);
}

Serv * User::getServ( void ) const {
	return (_serv);
}

int User::getStop( void ) const {
	return (_stop);
}

// OTHER PUBLIC MEMBER FUNCTIONS ===============================================

int User::rpl(int num, str const & p1, str const & p2)
{
	user_send(Msg(num, this, p1, p2));
	return (0);
}

int User::error(str const & msg)
{
	user_send(Msg("", "ERROR", msg));
	return (1);
}

void User::broadcast(Msg const & msg)
{
	std::set<User *> contacts;

	std::map<str, Chan *>::const_iterator chan_it;
	Chan::iterator user_it;
	for (chan_it = _chans.begin(); chan_it != _chans.end(); ++chan_it) {
		Chan * chan = chan_it->second;

		for (user_it = chan->begin(); user_it != chan->end(); ++user_it)
			contacts.insert(user_it->second);
	}

	contacts.erase(this);

	std::set<User *>::iterator it;
	for (it = contacts.begin(); it != contacts.end(); ++it)
		(*it)->user_send(msg);
}

void User::do_stuff( void )
{
	if (_stop)
		return ;

	int len = recv(_fd, _cbuffer, RECV_BUFF_SIZE - 1, 0);

	if (len <= 0) {
		broadcast(Msg(_pref, "QUIT", ":Connection closed"));
		OUT << C_MAGENTA << getNick() << " was disconnected" C_R << NL;
		_stop = -1;
		return ;
	}

	_cbuffer[len] = '\0';
	_ibuffer.append(_cbuffer);

	while (_ibuffer.find("\r\n") != str::npos) {
		if (_exec_cmd()) {
			OUT << C_MAGENTA << getNick() << " left" C_R << NL;
			_stop = 1;
			break ;
		}
	}
}

void User::user_send(Msg const & msg)
{
	OUT << C_BLUE << getNick() << " <\e[0;34m " << msg.getStr() << C_R;
	if (_obuffer.empty())
		_serv->epoll_register(_fd, EPOLLIN | EPOLLOUT, EPOLL_CTL_MOD);
	_obuffer += msg.getStr();
}

int	User::flush( void )
{
	if (_obuffer.empty())
		return (0);
	_serv->epoll_register(_fd, EPOLLIN, EPOLL_CTL_MOD);
	int ret = send(_fd, _obuffer.c_str(), _obuffer.size(), MSG_NOSIGNAL);
	_obuffer.clear();
	return (ret);
}