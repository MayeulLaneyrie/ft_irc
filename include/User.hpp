/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 18:47:46 by mlaneyri         ###   ########.fr       */
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
		int			_id;

		std::string _name;
		std::string _nick;

		std::string _ibuffer;

	public :

		User(void);
		User(int fd, int id);
		User(User const & src);
		~User(void);

		User & operator=(User const & rhs);
};

#endif

