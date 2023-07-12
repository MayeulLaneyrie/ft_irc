/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 17:39:48 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>

#include <vector>
#include <map>

#include "Chan.hpp"
#include "Client.hpp"

#define OPER_PASS "NECPLVSVLTRA"

class Server {

	private :

		int			_port;
		std::string	_password;

		/*
		 * chans and clients both identified by a unique numeric id 
		 * established from max_{client_id|chan_id}
		 */
		std::map<int, Chan>		_chans;
		std::map<int, Client>	_clients;

		int	_max_client_id;
		int _max_chan_id;

	public :

		Server(void);
		Server(int port, std::string password);
		Server(Server const & src);

		~Server(void);

		Server & operator=(Server const & rhs);

		int	run(void); // main loop

		Client & getClient(int id);
		Chan & getChan(int id);

};

#endif

