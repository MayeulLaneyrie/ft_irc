/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 16:11:07 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
#define MSG_HPP

#include <string>
#include <iostream>

#include <map>

#include "Chan.hpp"
#include "Client.hpp"

class Msg {

	private :

		Client * _from;
		Chan * _to;
		std::string _payload;

	public :

		Msg( void );
		Msg(Msg const & src);
		~Msg( void );

		Msg&	operator=(Msg const & rhs);
};

#endif

