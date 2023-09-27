/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:00:00 by mlaneyri          #+#    #+#             */
/*   Updated. 2023/07/13 16:00:00 by mlaneyri         ###   ########.fr       */
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

#define RPL_WELCOME ":Welcome to the {nw} Network, {n}[!{u}@{h}]"
#define RPL_YOURHOST ":Your host is {sn}, running version {v}"
#define RPL_CREATED ":This server was created {dt}"
#define RPL_MYINFO "{sn} {v} {aum} {acm}"

#define ERR_UNKNOWNCOMMAND "{1} :Unknown command"
#define ERR_NONICKNAMEGIVEN ":No nickname given"
#define ERR_ERRONEUSNICKNAME "{1} :Erroneus nickname"
#define ERR_NICKNAMEINUSE "{1} :Nickname is already in use"
#define ERR_NOTREGISTERED ":You have not registered"
#define ERR_NEEDMOREPARAMS "{1} :Not enough parameters"
#define ERR_ALREADYREGISTERED ":You may not reregister"

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

		std::map<int, str> _gen_rpl_map(void);

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
		Msg(int num, User * contact, str const & p1);

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

		str_vec payloadAsVector(int ac) const;

		int msg_send(void) const;
};

#endif
