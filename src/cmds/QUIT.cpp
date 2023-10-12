/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:16:25 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:24:29 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_QUIT(Msg & cmd) // ----------------------------------------- QUIT
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		arg.push_back("Client exited");
	broadcast(Msg(_nick, "QUIT", str(":Quit: ") + arg[0]));
	error(str(":Okay, bye: (") + _username + "@whatever) [" + arg[0] + ']');
	return (1);
}