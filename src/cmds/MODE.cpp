/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 21:20:15 by lnr               #+#    #+#             */
/*   Updated: 2023/10/11 21:20:16 by lnr              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

str mode_str(unsigned int bitset, const char * charset, std::map<char, str> vars)
{
	unsigned int i;
	unsigned int mask = 1;
	str char_str = "";
	str var_str = "";

	for (i = 0; charset[i] && i < 8 * sizeof(unsigned int) - 1; ++i) {
		if (bitset & mask) {
			char_str += charset[i];
			if (vars.count(charset[i]))
				(var_str += ' ') += vars[charset[i]];
		}
		mask *= 2;
	}
	return (char_str + var_str);
}

str int_to_str(int n)
{
	std::ostringstream oss;

	oss << n;
	return (oss.str());
}

int has_third(str_vec const & arg) {
	return (arg.size() == 3 && !arg[2].empty());
}

int User::_cmd_MODE(Msg & cmd) // ----------------------------------------- MODE
{
	str_vec arg = cmd.payloadAsVector(3);
	if (!arg.size())
		return (rpl(ERR_NEEDMOREPARAMS, "MODE"));

	if (is_name_nick(arg[0])) {
		if (!_serv->getUser(arg[0]))
			return (rpl(ERR_NOSUCHNICK, arg[0]));
		if (arg[0] != _nick)
			return (rpl(ERR_USERSDONTMATCH));
		if (_is_op)
			return (rpl(RPL_UMODEIS, "o"));
		return (rpl(RPL_UMODEIS, ""));
	}

	Chan * chan = _serv->getChan(arg[0]);

	if (!chan)
		return (rpl(ERR_NOSUCHCHANNEL, arg[0]));

	if (arg.size() == 1) {
		str mode_rpl = arg[0] + " +";

		std::map<char, str> mode_args;
		if (chan->getUser(_nick))
			mode_args['k'] = chan->getPasswd();
		else
			mode_args['k'] = "<key>";
		mode_args['l'] = int_to_str(chan->getLimit());

		mode_rpl += mode_str(chan->getMode(), "itkl", mode_args);
		return (rpl(RPL_CHANNELMODEIS, mode_rpl));
	}

	if (!chan->isOperator(this))
		return (rpl(ERR_CHANOPRIVSNEEDED, arg[0]));

	str allowed_chars = "itklo";
	str result = "";
	str result_args = "";
	int add = 1;

	for (str::iterator it = arg[1].begin() ; it != arg[1].end(); ++it) {
		if (*it == '+') {
			add = 1;
			result.push_back('+');
		}
		else if (*it == '-') {
			add = 0;
			result.push_back('-');
		}
		else if (*it == 'i') {
			chan->setMode(MODE_I, add);
			result.push_back('i');
		}
		else if (*it == 't') {
			chan->setMode(MODE_T, add);
			result.push_back('t');
		}
		else if (*it == 'k' && (!add || has_third(arg))) {
			result.push_back('k');
			chan->setMode(MODE_K, add);
			if (add) {
				chan->setPasswd(extract_first_word(arg[2]));
				(result_args += ' ') += chan->getPasswd();
			}
		}
		else if (*it == 'l' && (!add || has_third(arg))) {
			result.push_back('l');
			chan->setMode(MODE_L, add);
			if (add) {
				chan->setLimit(atoi(extract_first_word(arg[2]).c_str()));
				(result_args += ' ') += int_to_str(chan->getLimit());
			}
		}
		else if (*it == 'o' && has_third(arg)) {
			str target_name = extract_first_word(arg[2]);
			User * target = _serv->getUser(target_name);
			if (!target) 
				rpl(ERR_NOSUCHNICK, target_name);
			else if (chan->getUser(target_name)) {
				result.push_back('o');
				chan->opMode(target, add);
				(result_args += ' ') += target_name;
			}
		}
		else
			rpl(ERR_UNKNOWNMODE, str(1, *it));
	}
	if (result.find_first_of(allowed_chars) != str::npos)
		chan->chan_send(NULL, Msg(_nick, "MODE", result + result_args));
	return (0);
}