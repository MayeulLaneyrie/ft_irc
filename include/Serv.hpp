/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 19:18:45 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_HPP
#define SERV_HPP

#include <string>
#include <iostream>

#include <map>

#include "network.hpp"
#include "Chan.hpp"
#include "User.hpp"

#define OPER_PASS "NECPLVSVLTRA"

class Serv {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

// SERVER DATA -----------------------------------------------------------------

		int			_port;
		std::string	_password;

		/*
		 * chans and users both identified by a unique numeric id 
		 * established from max_{user_id|chan_id}
		 */
		std::map<int, Chan>	_chans;
		std::map<int, User>	_users;

		int	_max_user_id;
		int _max_chan_id;

		int _socket;

// INTERNAL STUFF  -------------------------------------------------------------

		int setup_socket(void);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

		Serv(void);
		Serv(int port, std::string password);
		Serv(Serv const & src);

		~Serv(void);

		Serv & operator=(Serv const & rhs);

// ACCESSORS -------------------------------------------------------------------

		User & getUser(int id);
		Chan & getChan(int id);

// OTHER PUBLIC MEMBERS FUNCTIONS ----------------------------------------------

		int	run(void); // main loop
		
};

#endif

