/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHOIS.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:19:57 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:19:58 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_WHOIS(Msg & cmd) // --------------------------------------- WHOIS
{
	str_vec arg = cmd.payloadAsVector(2, 1);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "WHOIS"));
	if (arg.size() == 2)
		arg[0] = arg[1];
	User *target = _serv->getUser(arg[0]);
	if (!target)
		return (rpl(ERR_NOSUCHNICK, arg[0]));
	std::map<str, Chan *> channelList = target->getChan();
	str listeChannel;
	for (std::map<str, Chan *>::const_iterator it = channelList.begin(); it != channelList.end(); it++) {
		if (it->second->isOperator(target))
			(listeChannel += '@') += it->first + " ";
		else
			listeChannel += it->first + " ";
	}
	rpl(RPL_WHOISUSER, target->getNick() + " " + target->getUsername(), target->getRealname());
	rpl(RPL_WHOISSERVER, target->getNick());
	if (target->getIsOp())
		rpl(RPL_WHOISOPERATOR, target->getNick());
	rpl(RPL_WHOISCHANNELS, target->getNick(), listeChannel);
	rpl(RPL_ENDOFWHOIS, target->getNick());
	return (0);
}