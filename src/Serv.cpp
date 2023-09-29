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

#include "utils.hpp"
#include "Serv.hpp"

// COPLIEN, CONSTRUCTORS & DESTRUCTORS -----------------------------------------

Serv::Serv(int port, std::string password) :
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

Serv::Serv(Serv const & src)
{
	*this = src;
}

Serv::~Serv(void)
{
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

void Serv::_clear(void)
{
	std::map<int, User *>::iterator user_it;
	for (user_it = _users.begin(); user_it != _users.end(); ++user_it) {
		user_it->second->error(":Server shutdown, bye bye!");
		user_it->second->flush();
		delete user_it->second;
	}

	std::map<std::string, Chan *>::iterator chan_it;
	for (chan_it = _chans.begin(); chan_it != _chans.end(); ++chan_it)
		delete chan_it->second;

	if (_epollfd >= 0)
		close(_epollfd);
	if (_sockfd >= 0)
		close(_sockfd);
}

int Serv::_setup_socket(void)
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

	return (0);
}

int Serv::_setup_epoll(void)
{
	if ((_epollfd = epoll_create(10)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));

	_epoll_register(_sockfd);
	return (0);
}

int	Serv::_epoll_register(int fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));
	return (0);
}

void Serv::_set_datetime(void)
{
	time_t curr_time;
	tm * curr_tm;
	char date_s[100];
	
	time(&curr_time);
	curr_tm = localtime(&curr_time);
	strftime(date_s, 50, "%Y/%m/%d %H:%M:%S", curr_tm);
	_datetime = str(date_s);
}

void Serv::_new_connection(void)
{
	int fd;

	socklen_t addrlen = sizeof(_sa);

	if ((fd = accept(_sockfd, (struct sockaddr *) &_sa, &addrlen)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));

	setsock_nonblock(fd);
	_epoll_register(fd);

	User * new_user = new User(this, fd);

	if (_users.count(fd)) // Somehow, a used fd was reattributed.
		die("sd", __FILE__, __LINE__);
	_users[fd] = new_user;
	_usercount++;
	std::cout
		<< "\e[1;46;30m" << new_user->getNick() << " joined.\e[0m" << std::endl;
}

void Serv::_user_manage(int fd)
{
	if (!_users.count(fd))
		die("sd", __FILE__, __LINE__);
	if (_users[fd]->user_recv())
		return ;
	_registerd.erase(_users[fd]->getNick());
	delete _users[fd];
	_users.erase(fd);
	_usercount--;
}

// ACCESSORS -------------------------------------------------------------------

User * Serv::getUserByNick(str const & nick)
{
	if (_registerd.count(nick))
		return (_registerd[nick]);
	return (NULL);
}

str Serv::getDatetime(void) const { return (_datetime); }

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

int Serv::run(void)
{
	_setup_socket();
	_setup_epoll();

	_set_datetime();
	signal(SIGINT, handler);
	std::cout	<< "\e[1;43;30mServer launched:\e[0m " << _datetime << std::endl
				<< "\e[1;43;30mListening on port:\e[0m " << _port << std::endl;

	while (1) {
		int nfds;
		struct epoll_event evts[MAXEV];

		if ((nfds = epoll_wait(_epollfd, evts, MAXEV, -1)) < 0 && !g_stop)
			die("sds", __FILE__, __LINE__, strerror(errno));

		if (g_stop)
			return (1);
		
		for (int i = 0; i < nfds; ++i) {
			int fd = evts[i].data.fd;

			if (fd == _sockfd)
				_new_connection();
			else
				_user_manage(fd);
		}
	}
	return (0);
}

int Serv::checkPass(std::string const & s) const
{
	return (s == _password);
}

void Serv::registerUser(User * user)
{
	_registerd[user->getNick()] = user;
}

void Serv::unregisterUser(User * user)
{
	_registerd.erase(user->getNick());
}
