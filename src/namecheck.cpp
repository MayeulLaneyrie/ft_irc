/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   namecheck.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 16:50:26 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/10 17:17:40 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "namecheck.hpp"

std::set<char> gen_badchar_set(void)
{
	std::set<char> ret;

	ret.insert(' ');
	ret.insert(',');
	ret.insert('*');
	ret.insert('?');
	ret.insert('!');
	ret.insert('.');
	ret.insert('@');
	ret.insert(':');
	ret.insert('$');
	
	return (ret);
}

int is_valid_name(str s)
{
	static const std::set<char> badchar_set = gen_badchar_set();

	if (s.size() > 9 || s.empty())
		return (0);

	for (str::iterator it = s.begin(); it != s.end(); ++it)
		if (badchar_set.count(*it) || *it < 20 || *it > 126)
			return (0);

	return (1);
}

int is_name_chan(str name)
{
	if (name.empty())
		return (0);
	return (name[0] == '#' || name[0] == '&');
}

int is_name_nick(str name)
{
	if (name.empty())
		return (0);
	return (name[0] != '#' && name[0] != '&');
}