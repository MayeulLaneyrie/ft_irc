/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:23:23 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/12 15:23:23 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "User.hpp"

int User::_cmd_NAMES(Msg & cmd)
{
	str_vec arg = cmd.payloadAsVector(1, 1);
	if (arg.size() < 1)
		return (rpl(RPL_ENDOFNAMES, "*"));
	
	str_vec names;
	while (!arg[0].empty()) {
		str tmp = extract_first_word(arg[0], ',');
		if (std::find(names.begin(), names.end(), tmp) == names.end())
			names.push_back(tmp);
	}

	for (str_vec::iterator i = names.begin(); i != names.end(); ++i) {

		Chan * chan = _serv->getChan(*i);
		if (!chan) {
			rpl(ERR_NOSUCHCHANNEL, *i);
			continue ;
		}
		
		str rpl_arg = *i + " :";
		for (Chan::iterator j = chan->begin(); j != chan->end(); ++j) {
			if (chan->isOp(j->second))
				(rpl_arg += '@') += j->second->getNick() + ' ';
			else
				rpl_arg += j->second->getNick() + ' ';
		}
		rpl(RPL_NAMREPLY, rpl_arg);
		rpl(RPL_ENDOFNAMES, *i);
	}
	return (0);
}