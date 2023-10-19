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

	if ((user = _serv->getUser(arg[0]))) {
		str rpl_param = arg[0] + " H" + (user->_is_op ? "*" : "")
			+ " :0 " + user->getRealname();

		rpl(RPL_WHOREPLY, user->getUsername(), rpl_param);
	}
	else if ((chan = _serv->getChan(arg[0]))) {
		Chan::iterator it;

		for (it = chan->begin(); it != chan->end(); ++it) {
			str rpl_param = it->first + " H" + (it->second->_is_op ? "*" : "")
				+ (chan->isOp(it->second) ? "@" : "") + " :0 " + it->second->getRealname();

			rpl(RPL_WHOREPLYCHAN, arg[0] + " " + it->second->getUsername(), rpl_param);
		}
	}
	return (rpl(RPL_ENDOFWHO, arg[0]));
}