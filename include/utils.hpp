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
#include <vector>

#include "network.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP

extern int g_stop;

typedef std::string str;
typedef std::vector<str> str_vec;

void die(const char * fmt, ...);

void setsock_nonblock(int fd);

str extract_first_word(str & s);

int sed(str & s, str const & from, str const & to);

void handler(int x);

#endif

