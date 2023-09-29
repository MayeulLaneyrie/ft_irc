/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/29 13:24:49 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <map>
#include <set>

#include "Chan.hpp"
#include "Msg.hpp"
#include "network.hpp"
#include "utils.hpp"

#define REG_MISM 8

#define REG_PASS 4
#define REG_NICK 2
#define REG_USER 1

#define REG_OK 7

#define CMD_NULL 0
#define CMD_PASS 1
#define CMD_NICK 2
#define CMD_USER 3
#define CMD_PING 4

class Chan;
class Msg;
class Serv;

class User {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

		typedef	int (User::*ft_cmd)(Msg &);

// USER DATA -------------------------------------------------------------------

		int		_fd;

		str		_nick;
		str		_username;
		str		_realname;
		
		str		_obuffer;
		str		_ibuffer;
		char	_cbuffer[RECV_BUFF_SIZE];

		int		_reg_status;

		std::map<str, Chan *> _chans;

		Serv *	_serv;

// INTERNAL STUFF --------------------------------------------------------------

		User(void);

		static std::map<str, ft_cmd> _gen_cmd_map(void);
		static std::set<str> _gen_prereg_set(void);

		int _exec_cmd(void);

		/*
		 * Here are all the command implementations. They are all located in
		 * cmds.cpp. They all take a Msg & as parameter, which has to contain
		 * the parsed command to execute. They each return 0 if, whatever
		 * happened when executing the command, the user should not be
		 * disconnected, and 1 if something happened, which has to lead to user
		 * disconnection (eg. wrong password).
		 */
		int _cmd_VOID(Msg & cmd);
		int _cmd_PASS(Msg & cmd);
		int _cmd_NICK(Msg & cmd);
		int _cmd_USER(Msg & cmd);
		int _cmd_PING(Msg & cmd);
		int _cmd_QUIT(Msg & cmd);
		int _cmd_PRIVMSG(Msg & cmd);
	//	int _cmd_NOTICE(Msg & cmd);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		User(Serv * serv, int fd);
		User(User const & src);
		~User(void);

		User & operator=(User const & rhs);

// ACCESSORS -------------------------------------------------------------------

		str getNick(void) const;
		str getUsername(void) const;

		Serv * getServ(void) const;

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		/*
		 * Send a reply to this user
		 */
		int rpl(int num, str const & p1 = "");

		int error(str const & msg);

		/*
		 * This user has send us some data. Let's do what has to be done.
		 */
		int user_recv(void);
		int user_send(Msg const & msg, int flushnow = 0);

		int	flush(void);
};

#endif

