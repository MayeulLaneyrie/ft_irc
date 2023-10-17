/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/11 21:29:48 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "Msg.hpp"
#include "network.hpp"
#include "utils.hpp"
#include "namecheck.hpp"

class Msg;

class Bot {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

// Bot DATA -------------------------------------------------------------------

		int _fd;

		str _nick;
		str _pass;

		str _ibuffer;
		char _cbuffer[RECV_BUFF_SIZE];

		int _is_connected;

		struct sockaddr_in _sa;

		std::set<str> _bad_words;

// INTERNAL STUFF --------------------------------------------------------------

		Bot( void );

		int _exec_cmd( void );

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		Bot(int port, str nick, str mdp);
		Bot(Bot const & src);
		~Bot( void );

		Bot & operator=(Bot const & rhs);

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

		int run( void );

		int send_to_serv(Msg const & msg);
};

#endif

