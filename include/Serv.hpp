/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/29 13:00:59 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_HPP
#define SERV_HPP

#include <string>
#include <iostream>

#include <ctime>

#include <signal.h>

#include <map>

#include "network.hpp"
#include "Chan.hpp"
#include "User.hpp"
#include "utils.hpp"

#define MAXEV 16

#define SERVER_NAME "ircserv"

#define OPER_NAME "LeBoss"
#define OPER_PASS "NECPLUSULTRA"

class User;
class Chan;

class Serv {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

// SERVER DATA -----------------------------------------------------------------

		int	_port;
		str	_password;

		str _datetime;

		std::map<int, User *> _users; // All users, uniquely indexed by fd
		std::map<str, User *> _registerd; // Only registered users, indexed by nicks

		std::map<str, Chan *> _chans;

		int	_usercount;
		int	_chancount;

		struct sockaddr_in6 _sa;
		int _sockfd;
		int _epollfd;

// INTERNAL STUFF  -------------------------------------------------------------

		void _clear(void);

		int _setup_socket(void);
		int _setup_epoll(void);

		int _epoll_register(int fd);

		void _set_datetime(void);

		void _new_connection(void);
		void _user_manage(int fd);

		Serv(void);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		Serv(int port, str password);
		Serv(Serv const & src);

		~Serv(void);

		Serv & operator=(Serv const & rhs);

// ACCESSORS -------------------------------------------------------------------

		User * getUserByNick(str const & nick);

		str getDatetime(void) const;

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		int	run(void); // main loop

		int checkPass(str const & s) const;

		void registerUser(User * user);
		void unregisterUser(User * user);
		void killUser(User * user);

		Chan * addChan(str name);

		void rmChan(str name);
		Chan * getChan(str name) const;
};

#endif

