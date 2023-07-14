/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chan.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:00:00 by mlaneyri          #+#    #+#             */
/*   Updated. 2023/07/13 16:00:00 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAN_HPP
#define CHAN_HPP

#include <string>
#include <iostream>

#include <map>

#include "User.hpp"
#include "Msg.hpp"

class Msg;

class Chan {

	private :

		std::map<std::string, User *>	_users; // indexed by their nicks
		
		std::string		_name;
		unsigned int	_mode;
		std::string		_passwd;
		std::string		_topic;

		Chan(void);

	public :

		Chan(std::string name);
		Chan(Chan const & src);
		~Chan(void);

		Chan & operator=(Chan const & rhs);

		int send(Msg const & msg);
};

#endif
