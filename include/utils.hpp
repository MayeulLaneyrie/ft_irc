/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/01 16:57:37 by mlaneyri         ###   ########.fr       */
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

std::string extract_cmd(std::string & s);

#endif

