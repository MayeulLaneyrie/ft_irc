/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:19:30 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:19:33 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_KICK(Msg & cmd) // ----------------------------------------- KICK
{

	str_vec arg = cmd.payloadAsVector(3);
	str comments;
	if (arg.size() < 2)
		return (rpl(ERR_NEEDMOREPARAMS, "KICK"));
	std::set<str> ChannelNames;
	std::set<str> UserNames;
	while (!arg[0].empty())
		ChannelNames.insert(extract_first_word(arg[0], ','));
	while (!arg[1].empty())
		UserNames.insert(extract_first_word(arg[1], ','));
	if (arg.size() == 3)
		comments = arg[2];
	std::set<str>::iterator it;
	std::set<str>::iterator it2;
	for (it = ChannelNames.begin(); it != ChannelNames.end(); it++)
	{
		Chan * channel = _serv->getChan(*it);
		if (!channel)
		{
			rpl(ERR_NOSUCHCHANNEL, *it);
			continue;
		}
		for (it2 = UserNames.begin(); it2 != UserNames.end(); it2++)
		{
			User *target = _serv->getUser(*it2);
			if (!target)
			{
				rpl(ERR_NOSUCHNICK, *it2);
				continue;
			}
			else if (!channel->getUser(target->getNick()))
				rpl(ERR_USERNOTINCHANNEL, target->getNick() + " " + channel->getName());
			else if (!channel->isOperator(this))
			{
				rpl(ERR_CHANOPRIVSNEEDED, channel->getName());
				continue ;
			}
			else
			{
				if (comments.empty())
					channel->chan_send(NULL, Msg(_nick, "KICK", channel->getName() + " :" + target->getNick() + " " + _nick ));
				else
					channel->chan_send(NULL, Msg(_nick, "KICK", channel->getName() + " :" + target->getNick() + " " + comments));
				channel->rmUser(target);
				target->rmChanFromList(channel->getName());
			}
		}
	}
	return 0;
}