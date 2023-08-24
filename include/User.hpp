/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/08/24 21:46:26 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>

#include "network.hpp"

class User {

	private :

		int			_fd;

		std::string _nick;
		std::string _ibuffer;

		char		_cbuffer[RECV_BUFF_SIZE];

		User(void);

	public :

		User(int fd);
		User(User const & src);
		~User(void);

		User & operator=(User const & rhs);

		/*
		 * This user sent data to the sevr, let's manage it.
		 */
		int do_stuff(void);
};

#endif

