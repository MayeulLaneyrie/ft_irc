/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:16:34 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/09/06 16:29:57 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void die(const char * fmt, ...) {
	va_list args;
	va_start(args, fmt);

	int d;
	char c;
	char *s;
	double f;

	while (*fmt) {
		switch (*fmt) {
			case 'd':
				d = va_arg(args, int);
				std::cerr << d;
				break ;
			case 'c':
				d = va_arg(args, int);
				c = d;
				std::cerr << c;
				break ;
			case 's':
				s = va_arg(args, char *);
				std::cerr << s;
				break ;
			case 'f':
				f = va_arg(args, double);
				std::cerr << f;
		}
		if (*(++fmt))
			std::cerr << ": ";
	}
	std::cerr << std::endl;
	exit(EXIT_FAILURE);
}

void setsock_nonblock(int fd) {
	int flags;

	if ((flags = fcntl(fd, F_GETFL)) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		die("sds", __FILE__, __LINE__, strerror(errno));
}

std::string extract_cmd(std::string & s)
{
	size_t cmd_end = s.find(" ");
	if (cmd_end == std::string::npos)
		cmd_end = s.size();
	std::string cmd = s.substr(0, cmd_end);
	s.erase(0, cmd_end + 1);
	return (cmd);
}

int sed(std::string & s, std::string const & from, std::string const & to)
{
	size_t pos = s.find(from);
	if (pos != std::string::npos) {
		s.erase(pos, from.size());
		s.insert(pos, to);
		return (1);
	}
	return (0);
}
