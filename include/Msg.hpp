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

#define RPL_WELCOME ":Welcome to the {nw} Network, {n}[!{u}@{h}]"
#define RPL_YOURHOST ":Your host is {sn}, running version {v}"
#define RPL_CREATED ":This server was created {dt}"
#define RPL_MYINFO "{sn} {v} {aum} {acm}"

#define ERR_NEEDMOREPARAMS "{1} :Not enough parameters"
#define ERR_ALREADYREGISTERED ":You may not reregister"

class Chan;
class User;

class Msg {

	private :

		std::string _prefix;
		std::string _cmd;
		std::string _payload;

		User * _contact;

	public :

		Msg( void );
	
		/*
		 * Construct message by explicitly specifying all informations
		 */
		Msg(User * contact, std::string const & pref, std::string const & cmd, std::string const & payld);

		/*
		 * Construct message from yet unparsed string
		 */
		Msg(User * contact, std::string const & s);

		/*
		 * Construct RPL message
		 */
		Msg(int num, User * contact, std::string const & p1, std::string const & p2);

		Msg(Msg const & src);

		~Msg( void );

		Msg & operator=(Msg const & rhs);

// ACCESSORS -------------------------------------------------------------------

		std::string getPrefix(void) const;
		std::string getCmd(void) const;
		std::string getPayload(void) const;

		User * getContact(void) const;

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		/*
		 * Returns the msg as a single string
		 */
		std::string str(void);

		std::vector<std::string> payloadAsVector(void) const;

		int msg_send(void);
};

#endif
