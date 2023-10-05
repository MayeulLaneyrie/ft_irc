/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 17:50:45 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/29 13:28:13 by mlaneyri         ###   ########.fr       */
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

	if (_serv->checkPass(arg[0]))
		_reg_status |= REG_PASS;
	else
		_reg_status |= REG_MISM;
	return (0);
}

// ------------------------------------------------------------------------ NICK

std::set<char> User::_gen_badchar_set(void)
{
	std::set<char> ret;

	ret.insert(' ');
	ret.insert(',');
	ret.insert('*');
	ret.insert('?');
	ret.insert('!');
	ret.insert('.');
	ret.insert('@');
	ret.insert(':');
	ret.insert('$');
	ret.insert('#');
	ret.insert('&');
	
	return (ret);
}

int User::_cmd_NICK(Msg & cmd)
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		return (rpl(ERR_NONICKNAMEGIVEN));

	if (_reg_status & REG_NICK && arg[0] == _nick)
		return (0);

	/*
	 * Rules:
	 *	- The nick has to be at most 9 chars long (and at least one, duh).
	 *	- The nick has to contain only printable ASCII chars.
	 *	- The nick shall not contain bad chars (as defined in _gen_badchar_set(), see higher).
	 */
	if (arg[0].size() > 9 || arg[0].empty())
		return (rpl(ERR_ERRONEUSNICKNAME, arg[0]));

	static const std::set<char> badchar_set = _gen_badchar_set();

	for (str::iterator it = arg[0].begin(); it != arg[0].end(); ++it)
		if (badchar_set.count(*it) || *it < 20 || *it > 126)
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

	user_send(
		Msg(this, SERVER_NAME, "PONG", str(SERVER_NAME) + " :" + arg[0])
	);

	return (0);
}

int User::_cmd_QUIT(Msg & cmd) // ----------------------------------------- QUIT
{
	str_vec arg = cmd.payloadAsVector(1);
	if (arg.empty())
		arg.push_back("Client exited");

	error(str(":Okay, bye: (") + _username + "@whatever) [" + arg[0] + ']');
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
			user_target->user_send(Msg(user_target, _nick, "PRIVMSG", *it + " :" + arg[1]));
		else if(chan_target)
			chan_target->chan_send(Msg(this, _nick, "PRIVMSG", *it + " :" + arg[1]));
		else
			rpl(ERR_NOSUCHNICK, *it);
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
	user_send(Msg(this, SERVER_NAME, "MODE", _nick + ": +o"));
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

	target->user_send(Msg(target, _nick, "KILL", arg[0] + " :" + arg[1]));
	target->error("Closing Link: " SERVER_NAME " (Killed (" + _nick + " (" + arg[1] + ")))");
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

	if (!channel)
	{
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
	channel->chan_send(Msg(NULL, _nick, "JOIN", str(":") + arg[0]));
	// TODO : RPLS
	return (0);
}

int User::_cmd_INVITE(Msg & cmd) //-------------------------------------- INVITE
{
	//check si le sender est register
	str_vec arg = cmd.payloadAsVector(2);
	if (arg.size() < 2)
		return (rpl(ERR_NEEDMOREPARAMS, "INVITE"));
	User *target = _serv->getUserByNick(arg[0]);
	if (!target)
		return (rpl(ERR_NOSUCHNICK, arg[0]));
	Chan *channel = _serv->getChan(arg[1]);
	if (!channel)
		return (rpl(ERR_NOSUCHCHANNEL, arg[1]));
	if (!channel->getUser(arg[0]))
		return (rpl(ERR_NOTONCHANNEL, arg[1]));
	if (channel->checkMode(MODE_I) && !channel->isOperator(target))
		return (rpl(ERR_CHANOPRIVSNEEDED, arg[1]));
	if (channel->getUser(arg[0]))
		return (rpl(ERR_USERONCHANNEL, arg[0] + " " + arg[1]));
	channel->invite(target);
	return (0);
	//check canal vers lequel on invite existe ERR_NOSUCHCHANNEL (403) (at least one user is on it)
	
	//personne envoyant le message est dans ce chan ERR_NOTONCHANNEL (442)

	// si chan en invite only seul un operateur peut inviter ERR_CHANOPRIVSNEEDED (482)
	
	//check user inviter n'est pas deja dans le chan ERR_USERONCHANNEL (443)

	//envoyer l'invitation et l'ajouter a la liste des membres inviter RPL_INVITING (341)
}
/*
int User::_cmd_WHOIS(Msg & cmd) //-----------------------------------------------WHOIS
{
	//ERR_NOSUCHNICK (401)

	//ERR_NOSUCHSERVER (402)

	//ERR_NONICKNAMEGIVEN (431)

	//RPL_ENDOFWHOIS (318)

	possible reply : 
	RPL_WHOISCERTFP (276)
	RPL_WHOISREGNICK (307)
	RPL_WHOISUSER (311)
	RPL_WHOISSERVER (312)
	RPL_WHOISOPERATOR (313)
	RPL_WHOISIDLE (317)
	RPL_WHOISCHANNELS (319)
	RPL_WHOISSPECIAL (320)
	RPL_WHOISACCOUNT (330)
	RPL_WHOISACTUALLY (338)
	RPL_WHOISHOST (378)
	RPL_WHOISMODES (379)
	RPL_WHOISSECURE (671)
	RPL_AWAY (301)
	
}

int User::_cmd_TOPIC(Msg & cmd) //---------------------------------------------TOPIC
{
	// 1 ou deux arguments (channel) (sujet du channel)
	//voir ou changer le topic du chan(si sujet du chan preciser)
	//client pas sur le chan ERR_NOTONCHANNEL (442) 
	//si topic protected et client veut le changer ERR_CHANOPRIVSNEEDED (482)
	//tous les user du chan recoivent les changements de topic
	//RPL_TOPIC (332) et RPL_TOPICWHOTIME (333)
}*/