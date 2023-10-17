/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/11 21:29:48 by lnr              ###   ########.fr       */
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
#include "namecheck.hpp"

// _reg_status bits significations:

#define REG_MISM 8 // cmd PASS has been called with a wrong password

#define REG_PASS 4 // cmd PASS has been successfully called
#define REG_NICK 2 // cmd NICK ...
#define REG_USER 1 // cmd USER ...

#define REG_OK 7 // All registrations cmds have been successfully called.
// If _reg_status == REG_OK and the REG_MISM bit isn't set to 1, the user is
// fully registered.

class Chan;
class Msg;
class Serv;

class User {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

		typedef	int (User::*ft_cmd)(Msg &);

// USER DATA -------------------------------------------------------------------

		int _fd;

		str _nick;
		str _pref;
		str _username;
		str _realname;
		
		/*
		 * output and input buffers. All input is bufferized to manage the
		 * case of partial command (which imply that a msg might require
		 * multiple calls to read()). All output is bufferized to avoid having
		 * more than one call to send for each epoll loop.
		 * _cbuffer is just the buffer used with recv().
		 */
		str _obuffer;
		str _ibuffer;
		char _cbuffer[RECV_BUFF_SIZE];

		/*
		 * _reg_status: bitset to indicate what conditions have been yet
		 * fulfilled during the registration process. See the REG_XXXX macros
		 * defined above.
		 * _is_op: boolean, self-explanatory.
		 */
		int _reg_status;
		int _is_op;

		std::map<str, Chan *> _chans;

		Serv * _serv;

// INTERNAL STUFF --------------------------------------------------------------

		User( void );

		/*
		 * Tables used in _exec_cmd()
		 */
		static std::map<str, ft_cmd> _gen_cmd_map( void );
		static const std::map<str, ft_cmd> _cmd_map;

		static std::set<str> _gen_prereg_set( void );
		static const std::set<str> _prereg_set;

		/*
		 * Extract a message from _ibuffer, parses it and then executes it.
		 * Returns 1 if it is then required to close this user's connection.
		 */
		int _exec_cmd( void );

		/*
		 * Here are all the command implementations. They are all located in
		 * their own file cmds/COMMAND.cpp. They all take a Msg & as parameter,
		 * which has to contain the parsed command to execute. They all return 1
		 * if the user's connection must be closed.
		 */
		int _cmd_VOID(Msg & cmd);
		int _cmd_PASS(Msg & cmd);
		int _cmd_NICK(Msg & cmd);
		int _cmd_USER(Msg & cmd);
		int _cmd_PING(Msg & cmd);
		int _cmd_QUIT(Msg & cmd);
		int _cmd_PRIVMSG(Msg & cmd);
		int _cmd_NOTICE(Msg & cmd);
		int _cmd_OPER(Msg & cmd);
		int _cmd_KILL(Msg & cmd);
		int _cmd_JOIN(Msg & cmd);
		int _cmd_INVITE(Msg & cmd);
		int _cmd_TOPIC(Msg & cmd);
		int _cmd_KICK(Msg & cmd);
		int _cmd_WHOIS(Msg & cmd);
		int _cmd_MODE(Msg & cmd);
		int _cmd_PART(Msg & cmd);
		int _cmd_NAMES(Msg & cmd);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		User(Serv * serv, int fd);
		User(User const & src);
		~User( void );

		User & operator=(User const & rhs);

// ACCESSORS -------------------------------------------------------------------

		/*
		 * That one does a bit more than just returning _nick: if no nick has
		 * been set yet, it returns a temporary nick in the form TMP.{fd}.
		 */
		str getNick( void ) const;
		str getUsername( void ) const;
		int getFd( void ) const;

		int isOper( void ) const;

		str getRealname( void ) const;
		std::map<str, Chan *> getChan( void ) const;
		void rmChanFromList(str name);

		Serv * getServ( void ) const;

		int	isFullyRegistered( void ) const;

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		/*
		 * Send a reply to this user, see the macros defined in Msg.hpp. Always
		 * returns 0, so it is possible in a _cmd_XXXX() to just
		 * return (rpl(ERR_WHATEVER)); p1 is an optionnal argument that may be
		 * required by some rpls (see Msg.cpp/_gen_rpl_map(), all messages
		 * that contains a "{1}" variable require p1 to be set). In the future,
		 * some rpls might require a second parametric argument (i.e. if some
		 * data is found to be quite awful to retrieve from the rpl constructor
		 * of Msg). In this case, a second parameter called p2 might be added.
		 */
		int rpl(int num, str const & p1 = "", str const & p2 = "");

		/*
		 * Send an error to this user, always returns 1, so it is possible in a
		 * _cmd_XXXX() to just return (error(":You did bad thing, bye."));
		 */
		int error(str const & msg);

		/*
		 * Sends a message all the users sharing a chan with this one.
		 * This is used to broadcast QUIT msgs to them when required.
		 */
		void broadcast(Msg const & msg);

		/*
		 * This user has send us some data. Let's do what has to be done.
		 * (Adding that data to the input buffer, and calling _exec_cmd() on it
		 * for every complete msg line it contains).
		 * Returns 0
		 */
		int user_recv( void );

		/*
		 * Adds msg at the end of the user's output buffer. If flushnow is set
		 * to 1, it is then immediatly flushed. The reason for this buffering is
		 * to avoid having more than 1 call to send() for each epoll loop.
		 */
		int user_send(Msg const & msg, int flushnow = 0);

		/*
		 * send() the output buffer (_obuffer) to the user, then clear it.
		 */
		int	flush( void );
};

#endif

