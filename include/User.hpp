/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/04 17:30:09 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <map>

#include "Chan.hpp"
#include "network.hpp"

#define REGISTER_PASS 4
#define REGISTER_NICK 2
#define REGISTER_USER 1

class Chan;

class User {

	private :

		int			_fd;

		std::string _nick;
		
		std::string _ibuffer;
		char		_cbuffer[RECV_BUFF_SIZE];

		int			_register_status;

		std::map<std::string, Chan *> _chans;

		// PRIVATE MEMBER FUNCTIONS --------------------------------------------

		User(void);

		void _exec_command(void);

	public :

		User(int fd);
		User(User const & src);
		~User(void);

		User & operator=(User const & rhs);

		int user_recv(void);
		int user_send(std::string const & s);
};

#endif

