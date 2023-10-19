/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/10/10 17:11:30 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

// COPLIEN =====================================================================

Bot::Bot(int port, str nick, str pass) :
	_nick(nick),
	_pass(pass),
	_is_connected(0)
{
	if (port < 1 || port > 65535)
		die("sdsd", __FILE__, __LINE__, "Invalid port value", port);

	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));

	_sa.sin_family = AF_INET;
	_sa.sin_addr.s_addr = htonl(0x7f000001);
	_sa.sin_port = htons(port);

	if (connect(_fd, (sockaddr*)&_sa, sizeof(_sa)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));

	send_to_serv(Msg("", "PASS", _pass));
	send_to_serv(Msg("", "NICK", _nick));
	send_to_serv(Msg("", "USER", "Bot 0 * :Bot"));
}

Bot::Bot(Bot const & src) {
	*this = src;
}

Bot::~Bot( void )
{
	close(_fd);
}

Bot & Bot::operator=(Bot const & rhs)
{
	close(_fd);
	_fd = rhs._fd;
	_nick = rhs._nick;
	_pass = rhs._pass;
	_ibuffer = rhs._ibuffer;
	_is_connected = rhs._is_connected;
	return (*this);
}

// INTERNAL STUFF ==============================================================

int Bot::_exec_cmd( void )
{
	size_t msg_len = _ibuffer.find("\r\n");
	str msg_str = _ibuffer.substr(0, msg_len);
	_ibuffer.erase(0, msg_len + 2);

	Msg msg(msg_str);
	str cmd = msg.getCmd();

	OUT << "\e[0;32m" << msg_str << C_R << NL;

	if (msg.getCmd() == "001")
		_is_connected = 1;
	else if (!_is_connected) {
		OUT << C_MAGENTA "Connection failed!" C_R << NL;
		return (1);
	}
	
	if (msg.getCmd() == "PING")
		send_to_serv(Msg("", "PONG", msg.getPayload()));

	str prefix = msg.getPrefix();
	str sender = extract_first_word(prefix, '!');

	if (msg.getCmd() != "PRIVMSG")
		return (0);

	str_vec arg = msg.payloadAsVector(2);
	if (arg.size() != 2)
		return (0);

	if (!is_name_chan(arg[0])) {

		str bot_cmd = extract_first_word(arg[1]);
		if (bot_cmd == "join")
			send_to_serv(Msg("", "JOIN", arg[1]));
		else if (bot_cmd == "banword") {
			notice(sender, "\"" + arg[1] + "\" will be added to the list of forbidden words.");
			_bad_words.insert(arg[1]);
		}
		else if (bot_cmd == "list") {
			str list_msg("The following words are currently banned: ");
			std::set<str>::iterator it;

			for (it = _bad_words.begin(); it != _bad_words.end(); ++it) {
				if (it != _bad_words.begin())
					list_msg += ", ";
				list_msg += *it;
			}
			notice(sender, list_msg);
		}
		else if (bot_cmd == "help") {
			notice(sender, "Hi, I am a censor bot! Whenever someone tells a bad"
				" word, I will tell them to watch their f*cking mouth.");
			notice(sender, "You can user me with the following commands:");
			notice(sender, "help: display these messages");
			notice(sender, "join <chan>: try to join the specified channel(s)");
			notice(sender, "banword <word>: ban the specified word");
			notice(sender, "list: display a list of all the banned words");
		}
		else
			notice(sender, "I don't know this command: " + bot_cmd);
		return (0);
	}

	std::set<str>::iterator it;
	for (it = _bad_words.begin(); it != _bad_words.end(); ++it) {
		if (arg[1].find(*it) != str::npos)
			notice(arg[0], "Watch your f*cking mouth " + sender + ", \"" + *it
				+ "\" is a forbidden word!");
	}

	return (0);
}

void Bot::_setup_sig( void )
{
	struct sigaction info;

	info.sa_handler = sighandler;
	info.sa_flags = 0;
	sigemptyset(&info.sa_mask);
	sigaction(SIGINT, &info, NULL);
}

// OTHER PUBLIC MEMBER FUNCTIONS ===============================================

int Bot::run( void )
{
	_setup_sig();

	while (1) {

		int len = recv(_fd, _cbuffer, RECV_BUFF_SIZE, 0);

		if (errno == EINTR)
			return (1);
		if (len <= 0) {
			OUT << C_MAGENTA "Connection lost." C_R << NL;
			return (1);
		}

		_cbuffer[len] = '\0';
		_ibuffer.append(_cbuffer);

		while (_ibuffer.find("\r\n") != str::npos)
			if (_exec_cmd()) {
				flush();
				return (1);
			}
		flush();
	}
}

void Bot::notice(str target, str msg) {
	send_to_serv(Msg("", "NOTICE", target + " :" + msg));
}

void Bot::send_to_serv(Msg const & msg)
{
	OUT << "\e[0;34m" << msg.getStr() << C_R;
	_obuffer += msg.getStr();
}

int Bot::flush( void )
{
	if (_obuffer.empty())
		return (0);
	int ret = send(_fd, _obuffer.c_str(), _obuffer.size(), MSG_NOSIGNAL);
	_obuffer.clear();
	return (ret);
}