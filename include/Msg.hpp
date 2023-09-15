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

#include "Chan.hpp"
#include "User.hpp"

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
	
		Msg(User * contact, std::string const & pref, std::string const & cmd, std::string const & payld);

		/*
		 * Construct RPL message
		 */
		Msg(int num, User * contact, std::string const & p1, std::string const & p2);

		Msg(Msg const & src);

		~Msg( void );

		Msg & operator=(Msg const & rhs);

		/*
		 * Returns the msg as a single string
		 */
		std::string str(void);

		int msg_send(void);
};

#endif

