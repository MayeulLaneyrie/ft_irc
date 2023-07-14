/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:00:00 by mlaneyri          #+#    #+#             */
/*   Updated. 2023/07/13 16:00:00 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
#define MSG_HPP

#include <string>
#include <iostream>

#include "Chan.hpp"
#include "User.hpp"

class Chan;

class Msg {

	private :

		std::string _from;
		std::string _to;
		std::string _payload;

	public :

		Msg( void );
		Msg(Msg const & src);
		~Msg( void );

		Msg & operator=(Msg const & rhs);
};

#endif

