/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:29:55 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/08/24 21:46:05 by mlaneyri         ###   ########.fr       */
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
	for (user_it = _users.begin(); user_it != _users.end(); ++user_it)
		delete user_it->second;

	std::map<std::string, Chan *>::iterator chan_it;
	for (chan_it = _chans.begin(); chan_it != _chans.end(); ++chan_it)
		delete chan_it->second;

	close(_epollfd);
	close(_sockfd);
}

int Serv::_setup_socket(void)
{
	_sa.sin_family = AF_INET;
	_sa.sin_port = htons(6667);
	_sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((_sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK , 0)) < 0)
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

// OTHER PUBLIC MEMBER FUNCTIONS -----------------------------------------------

int Serv::run(void)
{
	std::cout << "Server launched!" << std::endl;
	_setup_socket();
	_setup_epoll();

	while (1) {
		int nfds;
		struct epoll_event evts[MAXEV];

		if ((nfds = epoll_wait(_epollfd, evts, MAXEV, -1)) < 0)
			die("sds", __FILE__, __LINE__, strerror(errno));
		
		for (int i = 0; i < nfds; ++i) {
			int fd = evts[i].data.fd;

			if (fd == _sockfd) { // NEW CONNECTION

				socklen_t addrlen = sizeof(_sa);

				if ((fd = accept(_sockfd, (struct sockaddr *) &_sa, &addrlen)) < 0)
					die("sds", __FILE__, __LINE__, strerror(errno));

				setsock_nonblock(fd);
				_epoll_register(fd);

				User * new_user = new User(fd);

				if (_users.count(fd)) // Somehow, a used fd was reattributed.
					die("sd", __FILE__, __LINE__);
				_users[fd] = new_user;
				std::cout << "A new user joined." << std::endl;

			}
			else {
				if (!_users.count(fd))
					die("sd", __FILE__, __LINE__);
				if (!_users[fd]->do_stuff()) {
					delete _users[fd];
					_users.erase(fd);
				}
			}
		}
	}

	return (0);
}


