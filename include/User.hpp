/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/12 17:07:41 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

#include "Chan.hpp"
#include "Msg.hpp"
#include "network.hpp"
#include "utils.hpp"

#define REG_MISM 8

#define REG_PASS 4
#define REG_NICK 2
#define REG_USER 1

#define REG_OK 7

#define CMD_PASS 0
#define CMD_NICK 1
#define CMD_USER 2
#define CMD_PING 3

class Chan;
class Msg;
class Serv;

class User {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

// USER DATA -------------------------------------------------------------------

		int		_fd;

		str 	_nick;
		
		str		_ibuffer;
		char	_cbuffer[RECV_BUFF_SIZE];

		int		_reg_status;

		std::map<str, Chan *> _chans;

		Serv *	_serv;

// INTERNAL STUFF --------------------------------------------------------------

		User(void);

		int	_register_cmd(Msg msg, int cmd_id);
		int _exec_command(void);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		User(Serv * serv, int fd);
		User(User const & src);
		~User(void);

		User & operator=(User const & rhs);

// ACCESSORS -------------------------------------------------------------------

		str getNick(void) const;

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		/*
		 * Send a reply to this user
		 */
		int rpl(int num, str const & p1 = "", str const & p2 = "");

		/*
		 * This user has send us some data. Let's do what has to be done.
		 */
		int user_recv(void);
		int user_send(Msg const & msg) const;
};

#endif

