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

#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdlib>
#include <cstdarg>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "network.hpp"

#define OUT			(std::cout)
#define NL			(std::endl)

#define C_RED		"\e[1;41;30m"
#define C_GREEN		"\e[1;42;30m"
#define C_YELLOW	"\e[1;43;30m"
#define C_BLUE		"\e[1;44;30m"
#define C_MAGENTA	"\e[1;45;30m"
#define C_CYAN		"\e[1;46;30m"
#define C_R			"\e[0m"
#define C_R_		"\e[0m "

typedef std::string str;
typedef std::vector<str> str_vec;

/*
 * Exits the program, displaying a formatted error message. Parameters might be
 * int or char*, they will all be displayed, separated by ": ". fmt MUST only
 * contain 's' or 'd' characters, to specify which parameters are int or char*
 */
void die(const char * fmt, ...);

void setsock_nonblock(int fd);

str extract_first_word(str & s, char sep = ' ');

int sed(str & s, str const & from, str const & to);

void sighandler(int x);

#endif

