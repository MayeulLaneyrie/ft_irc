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
	if (arg.size() < 2)
		return (rpl(ERR_NEEDMOREPARAMS, "KICK"));

	Chan * channel = _serv->getChan(arg[0]);
	if (!channel)
		return (rpl(ERR_NOSUCHCHANNEL, arg[0]));

	std::set<str> UserNames;
	while (!arg[1].empty())
		UserNames.insert(extract_first_word(arg[1], ','));

	str comment = (arg.size() == 3 ? arg[2] : _nick);

	std::set<str>::iterator it;
	for (it = UserNames.begin(); it != UserNames.end(); it++) {

		User *target = _serv->getUser(*it);

		if (!target)
			rpl(ERR_NOSUCHNICK, *it);
		else if (!channel->getUser(_nick))
			rpl (ERR_NOTONCHANNEL, arg[0]);
		else if (!channel->getUser(*it))
			rpl(ERR_USERNOTINCHANNEL, *it + " " + arg[0]);
		else if (!channel->isOp(this))
			rpl(ERR_CHANOPRIVSNEEDED, arg[0]);
		else {
			channel->chan_send(Msg(_pref, "KICK", arg[0] + " " + *it + " :" + comment));
			channel->rmUser(target);
			target->rmChanFromList(arg[0]);
		}
	}
	return 0;
}