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

class User;
class Msg;

class Chan {

	private :

		std::map<str, User *>	_users; // indexed by their nicks
		
		unsigned int	_mode;
		str		_name;
		str		_passwd;
		str		_topic;

		Chan(void);

	public :

		Chan(str name);
		Chan(Chan const & src);
		~Chan(void);

		Chan & operator=(Chan const & rhs);

		int chan_send(Msg const & msg);

		int addUser(User & user);
		int rmUser(User const & user);
		User * getUser(str nick) const;

};

#endif
