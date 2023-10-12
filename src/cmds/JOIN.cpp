/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:18:23 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:18:24 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_JOIN(Msg & cmd) // ----------------------------------------- JOIN
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "JOIN"));
	str_vec ChannelNames;
	str_vec Key;
	std::vector<str>::iterator it;
	std::vector<str>::iterator keyIt;
	while (!arg[0].empty())
		ChannelNames.push_back(extract_first_word(arg[0], ','));
	if (arg.size() == 2)
		{
			while (!arg[1].empty())
				Key.push_back(extract_first_word(arg[1], ','));
			keyIt = Key.begin();
		}
	for (it = ChannelNames.begin(); it != ChannelNames.end(); it++)
	{
		Chan *channel = _serv->getChan(*it);
		if (!channel)
		{
			if (is_valid_name(*it) || is_name_chan(*it))
			{
				channel = _serv->addChan(*it);
				channel->opMode(this, 1);
			}
		}
		if (channel)
		{
			if (channel->getUser(_nick))
				;
			else if (channel->checkMode(MODE_I) && !channel->isInvited(this))
				rpl(ERR_INVITEONLYCHAN, *it);
			else if (channel->checkMode(MODE_K) && (arg.size() < 2 || !channel->checkPasswd(*keyIt)))
				rpl(ERR_BADCHANNELKEY, *it);
			else if (channel->checkMode(MODE_L) && channel->isFull())
				(rpl(ERR_CHANNELISFULL, *it));
			else
			{
				channel->addUser(this);
				_chans[*it] = channel;
				channel->chan_send(NULL, Msg(_nick, "JOIN", ":" + *it));
				if (channel->getTopic() != "")
					rpl(RPL_TOPIC, arg[0] + " :" + channel->getTopic());
				_cmd_NAMES("", "", *it);
			}
			if (arg.size() == 2 && keyIt != Key.end())
				keyIt++;
		}
	}
	return (0);
}
//verif du nom du chan,
//envoyer rpl topic
//liste de key aussi
//ajouter liste des user + @ si opetateur (RPL)
//@ sur whois si OP
//ajouter invite a la cmd