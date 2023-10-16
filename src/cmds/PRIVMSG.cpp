/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:17:00 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:17:01 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_PRIVMSG(Msg & cmd) // ----------------------------------- PRIVMSG
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() != 2)
		return (rpl(ERR_NEEDMOREPARAMS, "PRIVMSG"));
	std::set<str> names;
	while (!arg[0].empty())
		names.insert(extract_first_word(arg[0], ','));
	std::set<str>::iterator it;
	for (it = names.begin(); it != names.end(); ++it) {
		User * user_target = _serv->getUser(*it);
		Chan * chan_target = _serv->getChan(*it);
		if (user_target && user_target->isFullyRegistered())
			user_target->user_send(Msg(_pref, "PRIVMSG", *it + " :" + arg[1]));
		else if(chan_target)
			chan_target->chan_send(Msg(_pref, "PRIVMSG", *it + " :" + arg[1]), this);
		else
			rpl(ERR_NOSUCHNICK, *it);
	}
	return (0);
}