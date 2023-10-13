/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   namecheck.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 16:50:29 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/10 17:17:08 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NAMECHECK_HPP
#define NAMECHECK_HPP

#include <set>
#include "utils.hpp"

std::set<char> gen_badchar_set( void );

/*
** Rules:
**	- The name has to be at most 9 chars long (and at least one, duh).
**	- The name has to contain only printable ASCII chars.
**	- The name shall not contain bad chars (as defined in gen_badchar_set()).
*/
int is_valid_name(str s);

int is_name_chan(str name);

int is_name_nick(str name);

#endif