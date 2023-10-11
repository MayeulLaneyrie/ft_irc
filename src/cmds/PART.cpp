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
	str reason;
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "PART"));
	if (arg.size() == 2)
		reason = arg[1];
	std::set<str> names;
	while (!arg[0].empty())
		names.insert(extract_first_word(arg[0], ','));
	std::set<str>::iterator it;
	for (it = names.begin(); it != names.end(); it++)
	{
		Chan * channel = _serv->getChan(*it);
		if (!channel)
			rpl(ERR_NOSUCHCHANNEL, *it);
		else if (!channel->getUser(this->_nick))
			rpl(ERR_NOTONCHANNEL, this->_nick);
		else
		{
			if (reason.empty())
				channel->chan_send(NULL, Msg(_nick, "PART", channel->getName()));
			else
				channel->chan_send(NULL, Msg(_nick, "PART", channel->getName() + " :" + reason));
			this->rmChanFromList(channel->getName());
			channel->rmUser(this);
		}
	}
	return (0);
}