/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 17:50:45 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/10 18:10:13 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::_cmd_VOID(Msg & cmd) // ----------------------------------------- VOID
{
	(void)cmd;
	return (0);
}

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

// ------------------------------------------------------------------------ NICK

int User::_cmd_NICK(Msg & cmd)
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		return (rpl(ERR_NONICKNAMEGIVEN));

	if (_reg_status & REG_NICK && arg[0] == _nick)
		return (0);

	if (!is_valid_name(arg[0]) || is_name_chan(arg[0]))
		return (rpl(ERR_ERRONEUSNICKNAME, arg[0]));

	if (_serv->getUserByNick(arg[0]))
		return (rpl(ERR_NICKNAMEINUSE, arg[0]));

	if (_reg_status & REG_USER && _reg_status & REG_MISM)
		return (error(":Access denied, wrong password"));
	if (_reg_status & REG_USER && !(_reg_status & REG_PASS))
		return (error(":Access denied, password wasn't provided"));

	std::cout << C_CYAN << getNick() << " --> " << arg[0] << C_R << std::endl;
	_serv->renameUser(this, arg[0]);
	std::map<str, Chan *>::iterator it;
	for (it = _chans.begin(); it != _chans.end(); ++it)
		it->second->renameUser(this, arg[0]);
	_nick = arg[0];
	if (_reg_status & REG_NICK)
		return (0);
	_reg_status |= REG_NICK;
	if (_reg_status & REG_USER) {
		rpl(RPL_WELCOME);
		rpl(RPL_YOURHOST);
		rpl(RPL_CREATED);
		rpl(RPL_MYINFO);
	}
	return (0);
}

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
	_reg_status |= REG_USER;
	if (_reg_status & REG_NICK) {
		rpl(RPL_WELCOME);
		rpl(RPL_YOURHOST);
		rpl(RPL_CREATED);
		rpl(RPL_MYINFO);
	}
	return (0);
}

int User::_cmd_PING(Msg & cmd) // ----------------------------------------- PING
{
	str_vec arg = cmd.payloadAsVector(1, 1);
	if (arg.empty())
		return (rpl(ERR_NEEDMOREPARAMS, "PING"));
	user_send(Msg(SERVER_NAME, "PONG", str(SERVER_NAME) + " :" + arg[0]));
	return (0);
}

int User::_cmd_QUIT(Msg & cmd) // ----------------------------------------- QUIT
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		arg.push_back("Client exited");
	if (arg.empty()) {
		broadcast(Msg(_nick, "QUIT", ":Quit: "));
		error(str(":Okay, bye: (") + _username + "@whatever) [Client exited]");
	}
	else {
		broadcast(Msg(_nick, "QUIT", str(":Quit: ") + arg[0]));
		error(str(":Okay, bye: (") + _username + "@whatever) [" + arg[0] + ']');
	}
	return (1);
}

int User::_cmd_PRIVMSG(Msg & cmd) // ----------------------------------- PRIVMSG
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() != 2)
		return (rpl(ERR_NEEDMOREPARAMS, "PRIVMSG"));
	std::set<str> names;
	while (!arg[0].empty())
		names.insert(extract_first_word(arg[0], ','));
	std::set<str>::iterator it;
	for (it = names.begin(); it != names.end(); ++it) {
		User * user_target = _serv->getUserByNick(*it);
		Chan * chan_target = _serv->getChan(*it);
		if (user_target && user_target->isFullyRegistered())
			user_target->user_send(Msg(_nick, "PRIVMSG", *it + " :" + arg[1]));
		else if(chan_target)
			chan_target->chan_send(this, Msg(_nick, "PRIVMSG", *it + " :" + arg[1]));
		else
			rpl(ERR_NOSUCHNICK, *it);
	}
	return (0);
}

int User::_cmd_NOTICE(Msg & cmd) // ------------------------------------- NOTICE
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() != 2)
		return (0);
	std::set<str> names;
	while (!arg[0].empty())
		names.insert(extract_first_word(arg[0], ','));
	std::set<str>::iterator it;
	for (it = names.begin(); it != names.end(); ++it) {
		User * user_target = _serv->getUserByNick(*it);
		Chan * chan_target = _serv->getChan(*it);
		if (user_target && user_target->isFullyRegistered())
			user_target->user_send(Msg(_nick, "NOTICE", *it + " :" + arg[1]));
		else if(chan_target)
			chan_target->chan_send(this, Msg(_nick, "NOTICE", *it + " :" + arg[1]));
	}
	return (0);
}

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

int User::_cmd_KILL(Msg & cmd) // ----------------------------------------- KILL
{
	if (!_is_op)
		return (rpl(ERR_NOPRIVILEGES));
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() != 2)
		return (rpl(ERR_NEEDMOREPARAMS, "KILL"));
	User * target = _serv->getUserByNick(arg[0]);
	if (!target)
		return (rpl(ERR_NOSUCHNICK, arg[0]));
	target->user_send(Msg(_nick, "KILL", arg[0] + " :" + arg[1]));
	target->broadcast(Msg(arg[0], "QUIT", ":(Killed (" + _nick + " (" + arg[1] + ")))"));
	target->error(":Closing Link: " SERVER_NAME " :(Killed (" + _nick + " (" + arg[1] + ")))");
	_serv->killUser(target);
	std::cout << C_MAGENTA << arg[0] << " has been killed." C_R << std::endl;
	return (0);
}

int User::_cmd_JOIN(Msg & cmd) // ----------------------------------------- JOIN
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "JOIN"));
	Chan * channel = _serv->getChan(arg[0]);
	if (!channel) {
		channel = _serv->addChan(arg[0]);
		channel->addOperator(this);
	}
	if (channel->getUser(_nick))
		return (0);
	if (channel->checkMode(MODE_I) && !channel->isInvited(this))
		return (rpl(ERR_INVITEONLYCHAN, arg[0]));
	if (channel->checkMode(MODE_K) && !channel->checkPasswd(arg[1]))
		return (rpl(ERR_BADCHANNELKEY, arg[0]));
	if (channel->checkMode(MODE_L) && !channel->isFull())
		return (rpl(ERR_CHANNELISFULL, arg[0]));
	channel->addUser(this);
	_chans[arg[0]] = channel;
	channel->chan_send(NULL, Msg(_nick, "JOIN", str(":") + arg[0]));
	return (0);
}

int User::_cmd_INVITE(Msg & cmd) //-------------------------------------- INVITE
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 2)
		return (rpl(ERR_NEEDMOREPARAMS, "INVITE"));
	User *target = _serv->getUserByNick(arg[-1]);
	if (!target)
		return (rpl(ERR_NOSUCHNICK, arg[0]));
	Chan *channel = _serv->getChan(arg[1]);
	if (!channel)
		return (rpl(ERR_NOSUCHCHANNEL, arg[1]));
	if (!channel->getUser(_nick))
		return (rpl(ERR_NOTONCHANNEL, arg[1]));
	if (channel->checkMode(MODE_I) && !channel->isOperator(this))
		return (rpl(ERR_CHANOPRIVSNEEDED, arg[1]));
	if (channel->getUser(arg[0]))
		return (rpl(ERR_USERONCHANNEL, arg[0] + " " + arg[1]));
	channel->invite(target);
	rpl(RPL_INVITING, target->getNick() + " " + channel->getName());
	target->user_send(Msg(_nick + "INVITE" + target->getNick() + " :" +channel->getName()));
	return (0);
}

int User::_cmd_TOPIC(Msg & cmd) // --------------------------------------- TOPIC
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "TOPIC"));
	Chan *channel = _serv->getChan(arg[0]);
	if (!channel)
		return (rpl(ERR_NOSUCHCHANNEL, arg[0]));
	if (!channel->getUser(this->_nick))
		return (rpl(ERR_NOTONCHANNEL, arg[0]));
	if (arg.size() == 1)
	{
		if (channel->getTopic().empty())
			return (rpl(RPL_NOTOPIC,arg[0]));
		return (rpl(RPL_TOPIC, arg[0] + " :" + channel->getTopic())); // RPL_TOPICWHOTIME also need to be send
	}

	if (channel->checkMode(MODE_T) && !channel->isOperator(this))
		return (rpl(ERR_CHANOPRIVSNEEDED, arg[0]));
	channel->setTopic(arg[1]);
	channel->chan_send(NULL, Msg(arg[0], "TOPIC", str(":") + arg[1]));
	return (0);
}

int User::_cmd_KICK(Msg & cmd) // ----------------------------------------- KICK
{

	str_vec arg = cmd.payloadAsVector(3);
	str comments;
	if (arg.size() < 2)
		return (rpl(ERR_NEEDMOREPARAMS, "KICK"));
	std::set<str> ChannelNames;
	std::set<str> UserNames;
	while (!arg[0].empty())
		ChannelNames.insert(extract_first_word(arg[0], ','));
	while (!arg[1].empty())
		UserNames.insert(extract_first_word(arg[1], ','));
	if (arg.size() == 3)
		comments = arg[2];
	std::set<str>::iterator it;
	std::set<str>::iterator it2;
	for (it = ChannelNames.begin(); it != ChannelNames.end(); it++)
	{
		Chan * channel = _serv->getChan(*it);
		if (!channel)
		{
			rpl(ERR_NOSUCHCHANNEL, *it);
			continue;
		}
		for (it2 = UserNames.begin(); it2 != UserNames.end(); it2++)
		{
			User *target = _serv->getUserByNick(*it2);
			if (!target)
			{
				rpl(ERR_NOSUCHNICK, *it2);
				continue;
			}
			else if (!channel->getUser(target->getNick()))
				rpl(ERR_USERNOTINCHANNEL, target->getNick() + " " + channel->getName());
			else if (!channel->isOperator(this))
				{
					rpl(ERR_CHANOPRIVSNEEDED, channel->getName());
					continue ;
				}
			else
			{
				if (comments.empty())
					channel->chan_send(NULL, Msg(_nick, "KICK", channel->getName() + " :" + target->getNick() + " " + _nick ));
				else
					channel->chan_send(NULL, Msg(_nick, "KICK", channel->getName() + " :" + target->getNick() + " " + comments));
				channel->rmUser(target);
				target->rmChanFromList(channel->getName());
			}
		}
	}
	return 0;
}

int User::_cmd_WHOIS(Msg & cmd) // --------------------------------------- WHOIS
{
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 1)
		return (rpl(ERR_NONICKNAMEGIVEN, arg[0]));
	User *target = _serv->getUserByNick(arg[0]);
	if (!target)
		return (rpl(ERR_NOSUCHNICK, arg[0]));
	std::map<str, Chan *> channelList = target->getChan();
	str listeChannel;
	for (std::map<str, Chan *>::const_iterator it = channelList.begin(); it != channelList.end(); it++)
		listeChannel += it->first + " ";
	rpl(RPL_WHOISUSER, target->getNick() + " " + target->getUsername(), target->getRealname());
	rpl(RPL_WHOISSERVER, target->getNick());
	if (target->getIsOp())
		rpl(RPL_WHOISOPERATOR, target->getNick());
	rpl(RPL_WHOISCHANNELS, target->getNick(), listeChannel);
	rpl(RPL_ENDOFWHOIS, target->getNick());
	return (0);
}

int User::_cmd_MODE(Msg & cmd) // ----------------------------------------- MODE
{
	str_vec arg = cmd.payloadAsVector(3);
	if (!arg.size())
		return (rpl(ERR_NEEDMOREPARAMS, "MODE"));

	if (is_name_nick(arg[0])) {
		if (!_serv->getUserByNick(arg[0]))
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

	str allowed_chars = "itkl";
	str result = "";
	str result_args = "";
	int add = arg[1][0] == '-';

	if (arg[1][0] == '-' || arg[1][0] == '+')
		arg[1].erase(0, 1);

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
				if (result.find('i') != str::npos)
					break ;
				chan->setMode(MODE_I, add);
				result.push_back('i');
				break ;
			case 't':
				if (result.find('i') != str::npos)
					break ;
				chan->setMode(MODE_T, add);
				result.push_back('t');
				break ;
			case 'k':
				if (result.find('k') != str::npos)
					break ;
				result.push_back('k');
				chan->setMode(MODE_K, add);
				if (!add || arg.size() != 3 || arg[2].empty())
					break ;
				chan->setPasswd(extract_first_word(arg[2]));
				(result_args += ' ') += chan->getPasswd();
				break ;
			case 'l':
				if (result.find('l') != str::npos)
					break ;
				result.push_back('l');
				chan->setMode(MODE_L, add);
				if (!add || arg.size() != 3 || arg[2].empty())
					break ;
				chan->setLimit(atoi(extract_first_word(arg[2]).c_str()));
				(result_args += ' ') += int_to_str(chan->getLimit());
			default:
				rpl(ERR_UNKNOWNMODE, str(1, *it));
		}
	}
	Msg(_nick, "MODE", result + result_args);
	return (0);
}

int User::_cmd_PART(Msg & cmd) // ----------------------------------------- PART
{
	str reason;
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 1)
		return (rpl(ERR_NEEDMOREPARAMS, "PART"));
	if (arg.size() == 2)
		reason = arg[1];
	std::set<str> names;
	while (!arg[0].empty())
		names.insert(extract_first_word(arg[0], ','));
	std::set<str>::iterator it;
	for (it = names.begin(); it != names.end(); it++)
	{
		Chan * channel = _serv->getChan(*it);
		if (!channel)
			rpl(ERR_NOSUCHCHANNEL, *it);
		else if (!channel->getUser(this->_nick))
			rpl(ERR_NOTONCHANNEL, this->_nick);
		else
		{
			if (reason.empty())
				channel->chan_send(NULL, Msg(_nick, "PART", channel->getName()));
			else
				channel->chan_send(NULL, Msg(_nick, "PART", channel->getName() + " :" + reason));
			channel->rmUser(this);
			this->rmChanFromList(channel->getName());
		}
	}
	return (0);
}