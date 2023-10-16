/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KILL.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:17:54 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:17:55 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_KILL(Msg & cmd) // ----------------------------------------- KILL
{
	if (!_is_op)
		return (rpl(ERR_NOPRIVILEGES));
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() != 2)
		return (rpl(ERR_NEEDMOREPARAMS, "KILL"));
	User * target = _serv->getUser(arg[0]);
	if (!target)
		return (rpl(ERR_NOSUCHNICK, arg[0]));
	target->user_send(Msg(_pref, "KILL", arg[0] + " :" + arg[1]));
	target->broadcast(Msg(arg[0], "QUIT", ":(Killed (" + _nick + " (" + arg[1] + ")))"));
	target->error(":Closing Link: " SERVER_NAME " :(Killed (" + _nick + " (" + arg[1] + ")))");
	_serv->killUser(target);
	std::cout << C_MAGENTA << arg[0] << " has been killed." C_R << std::endl;
	return (0);
}