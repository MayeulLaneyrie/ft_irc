/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:14:15 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:14:17 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_PASS(Msg & cmd) // ----------------------------------------- PASS
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		return (rpl(ERR_NEEDMOREPARAMS, "PASS"));

	if (_reg_status == REG_OK)
		return (rpl(ERR_ALREADYREGISTERED));

	if (_serv->checkPass(arg[0])) {
		_reg_status |= REG_PASS;
		_reg_status &= ~REG_MISM;
	}
	else
		_reg_status |= REG_MISM;
	return (0);
}