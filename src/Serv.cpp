/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/29 13:32:03 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

Serv::Serv(int port, str const & password) :
	_password(password),
	_usercount(0),
	_chancount(0),
	_sockfd(-1),
	_epollfd(-1)
{
	if (port < 1 || port > 65535)
		die("sdsd", __FILE__, __LINE__, "Invalid port value", port);
	_port = port;
}

Serv::Serv(Serv const & src) {
	*this = src;
}

Serv::~Serv( void ) {
	_clear();
}

Serv & Serv::operator=(Serv const & rhs) {
	_clear();
	_port = rhs._port;
	_password = rhs._password;
	_datetime = rhs._datetime;
	_usercount = rhs._usercount;
	_chancount = rhs._chancount;
	_chans = rhs._chans;
	_users = rhs._users;
	_sockfd = rhs._sockfd;
	_epollfd = rhs._epollfd;
	return (*this);
}

// INTERNAL STUFF --------------------------------------------------------------

void Serv::_clear( void )
{
	std::map<int, User *>::iterator user_it;
	for (user_it = _users.begin(); user_it != _users.end(); ++user_it)
		delete (user_it->second);

	std::map<str, Chan *>::iterator chan_it;
	for (chan_it = _chans.begin(); chan_it != _chans.end(); ++chan_it)
		delete (chan_it->second);

	if (_epollfd >= 0)
		close(_epollfd);
	if (_sockfd >= 0)
		close(_sockfd);
}

void Serv::_setup_socket( void )
{
	int optval = 1;
	_sa.sin6_family = AF_INET6;
	_sa.sin6_port = htons(_port);
	_sa.sin6_addr = in6addr_any;
	_sa.sin6_flowinfo = 0;
	_sa.sin6_scope_id = 0;

	if ((_sockfd = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK , 0)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));
	if (bind(_sockfd, (struct sockaddr *) &_sa, sizeof(_sa)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));
	if (listen(_sockfd, 64) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));

	setsock_nonblock(_sockfd);
}

void Serv::_setup_datetime( void )
{
	time_t curr_time;
	tm * curr_tm;
	char date_s[100];
	
	time(&curr_time);
	curr_tm = localtime(&curr_time);
	strftime(date_s, 50, "%Y/%m/%d %H:%M:%S", curr_tm);
	_datetime = str(date_s);
}

void Serv::_setup_all( void )
{
	_setup_socket();
	_setup_datetime();

	if ((_epollfd = epoll_create(10)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));
	epoll_register(_sockfd, EPOLLIN, EPOLL_CTL_ADD);

	signal(SIGINT, sighandler);
}

void Serv::_new_connection( void )
{
	int fd;

	socklen_t addrlen = sizeof(_sa);

	if ((fd = accept(_sockfd, (struct sockaddr *) &_sa, &addrlen)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));

	setsock_nonblock(fd);
	epoll_register(fd, EPOLLIN, EPOLL_CTL_ADD);

	User * new_user = new User(this, fd);

	_users[fd] = new_user;
	_usercount++;
	OUT << C_CYAN << new_user->getNick() << " joined" C_R << NL;
}

void Serv::_manage_event(struct epoll_event ev)
{
	if (ev.data.fd == _sockfd) {
		_new_connection();
		return ;
	}

	User * user = _users[ev.data.fd];

	if (ev.events & EPOLLIN) {
		if (!user->getStop())
			user->do_stuff();
		if (user->getStop() < 0)
			killUser(user);
	}

	if (ev.events & EPOLLOUT) {
		if (user->getStop() >= 0)
			user->flush();
		if (user->getStop())
			killUser(user);
	}
}

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

int Serv::run( void )
{
	int stop = 0;

	_setup_all();

	OUT << C_YELLOW "Server launched:  " C_R_ << _datetime << NL
		<< C_YELLOW "Listening on port:" C_R_ << _port << NL;

	while (stop < 3)
	{
		int nfds;
		struct epoll_event evts[MAXEV];

		if ((nfds = epoll_wait(_epollfd, evts, MAXEV, -1)) < 0 && !stop)
		{
			OUT << C_RED "*** The server will now stop ***" C_R << NL;

			if (!_usercount)
				return (1);

			std::map<int, User *>::iterator user_it;
			for (user_it = _users.begin(); user_it != _users.end(); ++user_it)
				user_it->second->error(":Server shutdown, bye bye!");

			stop = 1;
		}

		if (stop)
			++stop;

		for (int i = 0; i < nfds; ++i)
			_manage_event(evts[i]);
	}
	return (0);
}

void Serv::epoll_register(int fd, int events, int op)
{
	struct epoll_event ev;

	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(_epollfd, op, fd, &ev) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));
}

str Serv::getDatetime( void ) const {
	return (_datetime);
}

int Serv::checkPass(str const & s) const {
	return (s == _password);
}

void Serv::renameUser(User * user, str const & to)
{
	_registerd.erase(user->getNick());
	_registerd[to] = user;
}

User * Serv::getUser(str const & nick)
{
	if (_registerd.count(nick))
		return (_registerd[nick]);
	return (NULL);
}

void Serv::killUser(User * user)
{
	_registerd.erase(user->getNick());
	_users.erase(user->getFd());
	delete (user);
	_usercount--;
}

Chan * Serv::addChan(str const & name)
{
	_chans[name] = new Chan(this, name);
	return (_chans[name]);
}

void Serv::rmChan(str const & name)
{
	if (!_chans.count(name))
		return ;
	Chan * chan = _chans[name];
	_chans.erase(name);
	delete (chan);
}

Chan * Serv::getChan(str const & name) const
{
	if (_chans.count(name))
		return (_chans.at(name));
	return (NULL);
}