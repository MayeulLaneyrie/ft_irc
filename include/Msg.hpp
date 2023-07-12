/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 18:48:36 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
#define MSG_HPP

#include <string>
#include <iostream>

#include "Chan.hpp"
#include "User.hpp"

class Msg {

	private :

		User * _from;
		Chan * _to;
		std::string _payload;

	public :

		Msg( void );
		Msg(Msg const & src);
		~Msg( void );

		Msg & operator=(Msg const & rhs);
};

#endif

