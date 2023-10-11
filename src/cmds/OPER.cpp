/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OPER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:17:37 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:17:43 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_OPER(Msg & cmd) // ----------------------------------------- OPER
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() != 2)
		return (rpl(ERR_NEEDMOREPARAMS, "OPER"));
	if (arg[0] != OPER_NAME)
		return (rpl(ERR_NOOPERHOST));
	if (arg[1] != OPER_PASS)
		return (rpl(ERR_PASSWDMISMATCH));
	_is_op = 1;
	user_send(Msg(SERVER_NAME, "MODE", _nick + ": +o"));
	return (rpl(RPL_YOUREOPER));
}