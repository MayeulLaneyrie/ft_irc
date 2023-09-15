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

#define REGISTER_PASS 4
#define REGISTER_NICK 2
#define REGISTER_USER 1

#define RPL_WELCOME ":Welcome to the {nw} Network, {n}[!{u}@{h}]"
#define RPL_YOURHOST ":Your host is {sn}, running version {v}"
#define RPL_CREATED ":This server was created {dt}"
#define RPL_MYINFO "{sn} {v} {aum} {acm}"
//#define RPL_ISUPPORT ":"

#define CMD_PASS 0
#define CMD_NICK 1
#define CMD_USER 2
#define CMD_PING 3

class Chan;
class Msg;

class User {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

// USER DATA -------------------------------------------------------------------

		int			_fd;

		std::string _nick;
		
		std::string _ibuffer;
		char		_cbuffer[RECV_BUFF_SIZE];

		int			_register_status;

		std::map<std::string, Chan *> _chans;

// INTERNAL STUFF --------------------------------------------------------------

		User(void);
		void _exec_command(void);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		User(int fd);
		User(User const & src);
		~User(void);

		User & operator=(User const & rhs);

// ACCESSORS

		std::string getNick(void) const;

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		int rpl(int num, std::string const & p1 = "", std::string const & p2 = "");

		int user_recv(void);
		int user_send(std::string const & s) const;
};

#endif

