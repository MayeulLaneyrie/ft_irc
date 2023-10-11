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
	Chan * channel = _serv->getChan(arg[0]);
	if (!channel) {
		channel = _serv->addChan(arg[0]);
		channel->opMode(this, 1);
	}
	if (channel->getUser(_nick))
		return (0);
	if (channel->checkMode(MODE_I) && !channel->isInvited(this))
		return (rpl(ERR_INVITEONLYCHAN, arg[0]));
	if (channel->checkMode(MODE_K) && (arg.size() < 2 || !channel->checkPasswd(arg[1])))
		return (rpl(ERR_BADCHANNELKEY, arg[0]));
	if (channel->checkMode(MODE_L) && channel->isFull())
		return (rpl(ERR_CHANNELISFULL, arg[0]));
	channel->addUser(this);
	_chans[arg[0]] = channel;
	channel->chan_send(NULL, Msg(_nick, "JOIN", str(":") + arg[0]));
	return (0);
}