/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/29 13:32:14 by mlaneyri         ###   ########.fr       */
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
	_serv(serv)
{}

User::User(User const & src) {
	*this = src;
}

User::~User(void)
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
	_ibuffer = rhs._ibuffer;
	_reg_status = rhs._reg_status;
	return (*this);
}

// INTERNAL STUFF ==============================================================

std::map<str, User::ft_cmd> User::_gen_cmd_map(void)
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
	ret["MODE"] = &User::_cmd_VOID;
	ret["PRIVMSG"] = &User::_cmd_PRIVMSG;
	ret["OPER"] = &User::_cmd_OPER;
	ret["KILL"] = &User::_cmd_KILL;
	ret["JOIN"] = &User::_cmd_JOIN;
	return (ret);
}

std::set<str> User::_gen_prereg_set(void)
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

int User::_exec_cmd(void)
{
	static const std::map<str, ft_cmd> cmd_map = _gen_cmd_map();
	static const std::set<str> prereg_set = _gen_prereg_set();

	size_t msg_len = _ibuffer.find("\r\n");
	str msg_str = _ibuffer.substr(0, msg_len);
	_ibuffer.erase(0, msg_len + 2);

	std::cout << C_GREEN << getNick() << " >" C_R_ << "{ " << msg_str << " }" << std::endl;

	Msg cmd_msg(this, msg_str);
	str cmd = cmd_msg.getCmd();

	if (_reg_status != REG_OK && !prereg_set.count(cmd))
		return (rpl(ERR_NOTREGISTERED));

	if (!cmd_map.count(cmd))
		return (rpl(ERR_UNKNOWNCOMMAND, cmd));

	return (this->*cmd_map.at(cmd))(cmd_msg);
}

std::set<User *> User::_in_shared_chans(void)
{
	std::set<User *> ret;
	std::map<str, Chan *>::const_iterator chan_it;
	std::map<str, User *>::const_iterator user_it;

	for (chan_it = _chans.begin(); chan_it != _chans.end(); ++chan_it) {
		Chan * chan = chan_it->second;

		for (user_it = chan->begin(); user_it != chan->end(); ++user_it)
			ret.insert(user_it->second);
	}
	return (ret);
}

// ACCESSORS ===================================================================

str User::getNick(void) const
{
	if (!_nick.empty())
		return (_nick);
	std::ostringstream oss("");
	oss << "TMP." << _fd;
	return (oss.str());
}

str User::getUsername(void) const {
	return _username;
}

int User::getFd(void) const {
	return _fd;
}

Serv * User::getServ(void) const {
	return _serv;
}

int	User::isFullyRegistered(void) const {
	return (_reg_status == REG_OK && !(_reg_status & REG_MISM));
}

// OTHER PUBLIC MEMBER FUNCTIONS ===============================================

int User::rpl(int num, str const & p1)
{
	Msg rpl(num, this, p1);

	rpl.msg_send();
	return (0);
}

int User::error(str const & msg)
{
	user_send(Msg(this, "", "ERROR", msg));
	return (1);
}

int User::user_recv(void)
{
	int len = recv(_fd, _cbuffer, RECV_BUFF_SIZE - 1, 0);
	
	if (!len) {
		std::cout << C_MAGENTA << getNick() << " was disconnected" C_R << std::endl;
		return (1);
	}

	_cbuffer[len] = '\0';
	_ibuffer.append(_cbuffer);

	int exit = 0;
	while (_ibuffer.find("\r\n") != str::npos) {
		if (_exec_cmd()) {
			std::cout << C_MAGENTA << getNick() << " left" C_R << std::endl;
			exit = 1;
			break ;
		}
	}
	flush();
	return (exit);
}

int User::user_send(Msg const & msg, int flushnow)
{
	std::cout << C_BLUE << getNick() << " <" C_R_ << msg.getStr();
	_obuffer += msg.getStr();
	if (flushnow)
		return (flush());
	return (0);
}

int	User::flush(void)
{
	if (_obuffer.empty())
		return (0);
	int ret = send(_fd, _obuffer.c_str(), _obuffer.size(), 0);
	_obuffer.clear();
	return (ret);
}