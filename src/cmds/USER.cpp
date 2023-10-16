/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:15:54 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:15:56 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_USER(Msg & cmd) // ----------------------------------------- USER
{
	str_vec arg = cmd.payloadAsVector(4, 1);
	if (arg.size() != 4) 
		return (rpl(ERR_NEEDMOREPARAMS, "USER"));

	if (_reg_status == REG_OK)
		return (rpl(ERR_ALREADYREGISTERED));

	if (_reg_status & REG_NICK && _reg_status & REG_MISM)
		return (error(":Access denied, wrong password"));
	if (_reg_status & REG_NICK && !(_reg_status & REG_PASS))
		return (error(":Access denied, password wasn't provided"));

	_username = arg[0];
	_realname = arg[3];
	_pref = _nick + '!' + _username + "@whatever";

	_reg_status |= REG_USER;

	if (_reg_status & REG_NICK) {
		rpl(RPL_WELCOME);
		rpl(RPL_YOURHOST);
		rpl(RPL_CREATED);
		rpl(RPL_MYINFO);
	}
	return (0);
}