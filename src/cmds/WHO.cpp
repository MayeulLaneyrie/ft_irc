/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 15:28:23 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/18 15:28:23 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_WHO(Msg & cmd) // ------------------------------------------- WHO
{
	str_vec arg = cmd.payloadAsVector(1, 1);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "WHO"));

	Chan * chan;
	User * user;

	if ((user = _serv->getUser(arg[0])))
		user_send(Msg(RPL_WHOREPLY, user, "*", (user->isOper() ? "*" : "")));
	else if ((chan = _serv->getChan(arg[0]))) {
		Chan::iterator it;
		for (it = chan->begin(); it != chan->end(); ++it) {
			str flags = (it->second->isOper() ? "*" : "");
			if (chan->isOp(it->second))
				flags += "@";
			user_send(Msg(RPL_WHOREPLY, it->second, arg[0], flags));
		}
	}
	return (rpl(RPL_ENDOFWHO, arg[0]));
}