/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chan.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 18:48:06 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAN_HPP
#define CHAN_HPP

#include <string>
#include <iostream>

#include <set>

#include "User.hpp"

class Chan {

	private :

		std::string		_name;
		std::set<int>	_users;
		unsigned int	_mode;
		std::string		_passwd;
		std::string		_topic;

	public :

		Chan(void);
		Chan(Chan const & src);
		~Chan(void);

		Chan & operator=(Chan const & rhs);
};

#endif
