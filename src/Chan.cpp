/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chan.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 18:01:09 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chan.hpp"

Chan::Chan(void):
	_name(""),
	_mode(0),
	_passwd(""),
	_topic("")
{}

Chan::Chan(Chan const & src)
{
	*this = src;
}

Chan::~Chan(void)
{}

Chan & Chan::operator=(Chan const & rhs)
{
	this->_name = rhs._name;
	this->_mode = rhs._mode;
	this->_passwd = rhs._passwd;
	this->_topic = rhs._topic;
	return (*this);
}
