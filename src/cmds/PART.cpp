/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:20:59 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:21:00 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_PART(Msg & cmd) // ----------------------------------------- PART
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "PART"));
	
	std::set<str> names;
	while (!arg[0].empty())
		names.insert(extract_first_word(arg[0], ','));

	str reason = (arg.size() == 2 ? str(" :") + arg[1] : "");

	std::set<str>::iterator it;
	for (it = names.begin(); it != names.end(); it++) {

		Chan * channel = _serv->getChan(*it);

		if (!channel)
			rpl(ERR_NOSUCHCHANNEL, *it);
		else if (!channel->getUser(_nick))
			rpl(ERR_NOTONCHANNEL, _nick);
		else {
			channel->chan_send(Msg(_pref, "PART", *it + reason));
			rmChanFromList(*it);
			channel->rmUser(this);
		}
	}
	return (0);
}