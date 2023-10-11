/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:16:08 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:16:11 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_PING(Msg & cmd) // ----------------------------------------- PING
{
	str_vec arg = cmd.payloadAsVector(1, 1);
	if (arg.empty())
		return (rpl(ERR_NEEDMOREPARAMS, "PING"));
	user_send(Msg(SERVER_NAME, "PONG", str(SERVER_NAME) + " :" + arg[0]));
	return (0);
}