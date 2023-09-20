/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/06 16:29:45 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <cstdarg>

#include <iostream>
#include <string>

#include "network.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP

void die(const char * fmt, ...);

void setsock_nonblock(int fd);

std::string extract_first_word(std::string & s);

int sed(std::string & s, std::string const & from, std::string const & to);

#endif

