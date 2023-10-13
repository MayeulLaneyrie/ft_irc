/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:00:00 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/10 18:09:48 by mlaneyri         ###   ########.fr       */
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
#define RPL_UMODEIS 221

#define RPL_WHOISUSER 311 //
#define RPL_WHOISSERVER 312 //
#define RPL_WHOISOPERATOR 313 //
#define RPL_ENDOFWHOIS 318//
#define RPL_WHOISCHANNELS 319 //

#define RPL_CHANNELMODEIS 324
#define RPL_NOTOPIC 331
#define RPL_TOPIC 332
#define RPL_INVITING 341
#define RPL_NAMREPLY 353
#define RPL_ENDOFNAMES 366
#define RPL_YOUREOPER 381

#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHCHANNEL 403
#define ERR_BADCHANNAME 1403 
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL 442 
#define ERR_USERONCHANNEL 443 
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464
#define ERR_CHANNELISFULL 471
#define ERR_UNKNOWNMODE 472
#define ERR_INVITEONLYCHAN 473
#define ERR_BADCHANNELKEY 475
#define ERR_NOPRIVILEGES 481
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_NOOPERHOST 491
#define ERR_USERSDONTMATCH 502

class Chan;
class User;

/*
 * Msg is LOVE, Msg is LIFE!
 * All data exchanged between clients & the server has to be in the form of Msg.
 *
 * A Msg is formed by three parts:
 * 1. A prefix (optionnal), which is always preceded by a ':' character, and
 * that indicates the origin of the msg. All msgs send by the server to users
 * has to be prefixed with the SERVER_NAME. All PRIVMSG forwarded to users have
 * to be prefixed with the emitter's name.
 * 2. A command (which might be a three-digit number in the case of a rpl)
 * 3. The payload: space separated arguments, the last of which might contain
 * spaces if it is prefixed with a ':' character, although some commands don't
 * require a ':' to include the rest of the message line in the final argument.
 * Due to the arguments splitting rules varying according to the command, the
 * payload is here stored in a single str.
 *
 * These three parts, & the arguments are all to be separated by a single space
 * (' ', ASCII #32).
 */

class Msg {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

		str _prefix;
		str _cmd;
		str _payload;

		/*
		 * The three parts of the Msg concatenated as a single str. To be able
		 * to provide it instantly, it is regenerated each time one of the three
		 * parts is modified.
		 */
		str _as_str;

// INTERNAL STUFF --------------------------------------------------------------

		/*
		 * Regenerate the _as_str member. Has to be called each time one the
		 * three parts has been modified.
		 */
		void _regen_str(void);

		/*
		 * generates a table of all the possible numeric replies payloads,
		 * indexed by the reply number.
		 */
		static std::map<int, str> _gen_rpl_map(void);

		Msg( void );

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		/*
		 * Construct message by explicitly specifying all informations
		 */
		Msg(str const & pref, str const & cmd, str const & payld);

		/*
		 * Construct message from a yet unparsed string. This one is used when
		 * we need to parse a message line received from a user.
		 */
		Msg(str const & s);

		/*
		 * Construct a RPL message. The prefix will be the SERVER_NAME, the
		 * command will be the reply number. The payload string will be taken
		 * from the table generated by _gen_rpl_map(), and all the variable
		 * specified in it will be substituted with their value. This include
		 * the "{1}" variable which will be substituted with the p1 parameter.
		 * That one has been thought to be able to include in rpl messages
		 * variables that might be difficult/impossible to retrieve any other
		 * way. If needed, we might consider added another one of such parameter
		 * (p2, for variable "{2}").
		 */
		Msg(int num, User * contact, str const & p1 = 0, str const & p2 = 0);

		Msg(Msg const & src);

		~Msg( void );

		Msg & operator=(Msg const & rhs);

// ACCESSORS -------------------------------------------------------------------

		str getPrefix( void ) const;
		str getCmd( void ) const;
		str getPayload( void ) const;

		str getStr( void ) const;

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		/*
		 * This one is an actual banger. Each command needs to retrieve a
		 * certain number of arguments from the payload. We specify this number
		 * with ac.
		 *
		 * If we haven't enough arguments in the payload, we will just
		 * return the ones we were able to grab. (Some commands might need
		 * various number of arguments, this behaviour allows them to ask for
		 * the max number, and then manage what they got the way they want).
		 *
		 * Some commands don't require a colon (':' character) to be added
		 * before the final argument for it to include the rest of the payload,
		 * including spaces. We actually consider this as the default behaviour:
		 * if requireColon is set to 1, the final argument has to be prefixed
		 * with ':' to include more than one word.
		 *
		 * In any case, if the final argument starts with a ':' character, that
		 * one will be erased.
		 */
		str_vec payloadAsVector(int ac, int requireColon = 0) const;
};

#endif
