/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/10 15:43:31 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef 
#define

#include <string>
#include <iostream>

class Client {

	private :

		int			_fd;
		std::string _name;
		std::string _nick;

	public :

		Client( void );
		Client();
		Client(Client const & src);
		~Client( void );

		Client&	operator=(Client const & rhs);
};

#endif

