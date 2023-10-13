/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:19:02 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:19:04 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_TOPIC(Msg & cmd) // --------------------------------------- TOPIC
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "TOPIC"));

	Chan *channel = _serv->getChan(arg[0]);
	if (!channel)
		return (rpl(ERR_NOSUCHCHANNEL, arg[0]));

	if (!channel->getUser(this->_nick))
		return (rpl(ERR_NOTONCHANNEL, arg[0]));

	if (arg.size() == 1) {
		if (channel->getTopic().empty())
			return (rpl(RPL_NOTOPIC,arg[0]));
		return (rpl(RPL_TOPIC, arg[0] + " :" + channel->getTopic())); // RPL_TOPICWHOTIME also needs to be send
	}

	if (channel->checkMode(MODE_T) && !channel->isOp(this))
		return (rpl(ERR_CHANOPRIVSNEEDED, arg[0]));

	channel->setTopic(arg[1]);
	channel->chan_send(Msg(_nick, "TOPIC", arg[0] + " :" + arg[1]));
	return (0);
}