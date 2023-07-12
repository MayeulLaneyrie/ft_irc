/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chan.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/06/22 15:39:41 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAN_HPP
#define CHAN_HPP

#include <string>
#include <iostream>

#include "Client.hpp"

class Chan {

	private :

		std::vector<Client &>	_clients;
		std::vector<>
		unsigned int			_modes;
		std::string				_passwd;
		std::string				_topic;
		std::string				_name;

	public :

		Chan( void );
		Chan();
		Chan(Chan const & src);
		~Chan( void );

		Chan&	operator=(Chan const & rhs);
};

#endif
