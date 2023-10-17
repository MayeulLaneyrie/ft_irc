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
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));

	_sa.sin_family = AF_INET;
	_sa.sin_addr.s_addr = inet_addr("127.0.0.1");
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

	std::cout << "\e[0;32m" << msg_str << C_R << std::endl;

	if (msg.getCmd() == "001")
		_is_connected = 1;
	else if (!_is_connected) {
		std::cout << C_MAGENTA "Connection failed!" C_R << std::endl;
		return (1);
	}

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
			send_to_serv(Msg("", "NOTICE",
				sender + " :\"" + arg[1] + "\" will be added to the list of forbidden words."));
			_bad_words.insert(arg[1]);
		}
		return (0);
	}

	std::set<str>::iterator it;
	for (it = _bad_words.begin(); it != _bad_words.end(); ++it) {
		if (arg[1].find(*it) != str::npos)
			send_to_serv(Msg("", "NOTICE",
				arg[0] + " :Watch your f*cking mouth, " + sender + ", \"" + *it
				+ "\" is a forbidden word!"));
	}

	return (0);
}

// OTHER PUBLIC MEMBER FUNCTIONS ===============================================


int Bot::run( void )
{
	while (1) {

		int len = recv(_fd, _cbuffer, RECV_BUFF_SIZE, 0);

		if (len <= 0) {
			std::cout << C_MAGENTA "Connection lost." C_R << std::endl;
			return (1);
		}

		_cbuffer[len] = '\0';
		_ibuffer.append(_cbuffer);

		while (_ibuffer.find("\r\n") != str::npos)
			if (_exec_cmd())
				return (1);
	}
}

int Bot::send_to_serv(Msg const & msg)
{
	std::cout << "\e[0;34m" << msg.getStr() << C_R;
	return (send(_fd, msg.getStr().c_str(), msg.getStr().size(), MSG_NOSIGNAL));
}