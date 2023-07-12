/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 17:52:03 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>

#include "network.hpp"

class Client {

	private :

		int			_fd;
		int			_id;

		std::string _name;
		std::string _nick;

		std::string _ibuffer;

	public :

		Client(void);
		Client(int fd, int id);
		Client(Client const & src);
		~Client(void);

		Client & operator=(Client const & rhs);
};

#endif

