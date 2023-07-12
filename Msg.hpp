/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/06/25 16:54:29 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef 
#define

#include <string>
#include <iostream>

#include <map>

#include <Client.hpp>

class Msg {

	private :

		Client * _from;
		Chan * _to;
		std::string _payload;

	public :

		Msg( void );
		Msg();
		Msg(Msg const & src);
		~Msg( void );

		Msg&	operator=(Msg const & rhs);
};

#endif

