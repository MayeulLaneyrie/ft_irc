/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/10 16:24:02 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>

#include <string>
#include <iostream>

#include "Chan.hpp"
#include "Client.hpp"

#define OPER_PASS ""

class Server {

	private :

		int							_port;
		std::string					_password;

		std::map<std::string, Chan>	_chans;
		std::map<int, Client>		_clients;

	public :

		server(void);
		server(int port, std::string password);
		server(server const & src);

		~server(void);

		server&	operator=(server const & rhs);

		int	run(void); // main loop

		Client const & getClient(int fd) const;
};

#endif

