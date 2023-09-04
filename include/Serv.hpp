/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/04 17:30:17 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_HPP
#define SERV_HPP

#include <string>
#include <iostream>

#include <signal.h>

#include <map>

#include "network.hpp"
#include "Chan.hpp"
#include "User.hpp"

#define MAXEV 16

#define OPER_PASS "NECPLVSVLTRA"

class Serv {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

// SERVER DATA -----------------------------------------------------------------

		int			_port;
		std::string	_password;

		std::map<int, User *> _users; // All users, uniquely indexed by fd
		std::map<std::string, User *> _registerd; // Only registered users, indexed by nicks

		std::map<std::string, Chan *> _chans;

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

		Serv(void);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		Serv(int port, std::string password);
		Serv(Serv const & src);

		~Serv(void);

		Serv & operator=(Serv const & rhs);

// OTHER PUBLIC MEMBERS FUNCTIONS ----------------------------------------------

		int	run(void); // main loop
		
};

#endif

