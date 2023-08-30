/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/08/30 17:03:01 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>

#include "network.hpp"

#define REGISTER_PASS 4
#define REGISTER_NICK 2
#define REGISTER_USER 1

class User {

	private :

		int			_fd;

		std::string _nick;
		std::string _ibuffer;

		int			_register_status;

		char		_cbuffer[RECV_BUFF_SIZE];

		User(void);

		int _is_command_complete(void);
		void _exec_command(void);

	public :

		User(int fd);
		User(User const & src);
		~User(void);

		User & operator=(User const & rhs);

		/*
		 * This user sent data to the servr, let's manage it.
		 */
		int do_stuff(void);
};

#endif

