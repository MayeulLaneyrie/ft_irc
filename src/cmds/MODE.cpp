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

	str allowed_chars = "itklo";
	str result = "";
	str result_args = "";
	int add = 1;

	for (str::iterator it = arg[1].begin() ; it != arg[1].end(); ++it) {
		switch (*it) {
			case '+':
				add = 1;
				result.push_back('+');
				break ;
			case '-':
				add = 0;
				result.push_back('-');
				break ;
			case 'i':
				chan->setMode(MODE_I, add);
				result.push_back('i');
				break ;
			case 't':
				chan->setMode(MODE_T, add);
				result.push_back('t');
				break ;
			case 'k':
				if (add && (arg.size() != 3 || arg[2].empty()))
					break ;
				result.push_back('k');
				chan->setMode(MODE_K, add);
				if (!add)
					break ;
				chan->setPasswd(extract_first_word(arg[2]));
				(result_args += ' ') += chan->getPasswd();
				break ;
			case 'l':
				if (add && (arg.size() != 3 || arg[2].empty()))
					break ;
				result.push_back('l');
				chan->setMode(MODE_L, add);
				if (!add)
					break ;
				chan->setLimit(atoi(extract_first_word(arg[2]).c_str()));
				(result_args += ' ') += int_to_str(chan->getLimit());
				break ;
			case 'o': {
				if (arg.size() != 3 || arg[2].empty())
					break ;
				str target_name = extract_first_word(arg[2]);
				User * target = _serv->getUser(target_name);
				if (!target) { // TODO CHECK SI PRESENT SUR CHAN
					rpl(ERR_NOSUCHNICK, target_name);
					break ;
				}
				result.push_back('o');
				chan->opMode(target, add);
				(result_args += ' ') += target_name;
				break ; }
			default:
				rpl(ERR_UNKNOWNMODE, str(1, *it));
		}
	}
	if (result.find_first_of(allowed_chars) != str::npos)
		chan->chan_send(NULL, Msg(_nick, "MODE", result + result_args));
	return (0);
}