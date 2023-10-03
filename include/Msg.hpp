/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:00:00 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/29 13:33:57 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
#define MSG_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Chan.hpp"
#include "User.hpp"
#include "Serv.hpp"
#include "utils.hpp"

#define RPL_WELCOME 1
#define RPL_YOURHOST 2
#define RPL_CREATED 3
#define RPL_MYINFO 4

#define ERR_NOSUCHNICK 401
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462

class Chan;
class User;

class Msg {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

		str _prefix;
		str _cmd;
		str _payload;

		str _as_str;

		User * _contact;

// INTERNAL STUFF --------------------------------------------------------------

		void _regen_str(void);

		static std::map<int, str> _gen_rpl_map(void);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		Msg( void );
	
		/*
		 * Construct message by explicitly specifying all informations
		 */
		Msg(User * contact, str const & pref, str const & cmd, str const & payld);

		/*
		 * Construct message from yet unparsed string
		 */
		Msg(User * contact, str const & s);

		/*
		 * Construct RPL message
		 */
		Msg(int num, User * contact, str const & p1 = 0);

		Msg(Msg const & src);

		~Msg( void );

		Msg & operator=(Msg const & rhs);

// ACCESSORS -------------------------------------------------------------------

		str getPrefix(void) const;
		str getCmd(void) const;
		str getPayload(void) const;

		User * getContact(void) const;

		str getStr(void) const;

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		str_vec payloadAsVector(int ac, int requireColon = 0) const;

		int msg_send(void) const;
};

#endif
