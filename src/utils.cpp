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
	char *s;

	std::cerr << C_RED "FATAL ERROR:" C_R_;
	while (*fmt) {
		if (*fmt == 'd') {
			d = va_arg(args, int);
			std::cerr << d;
		}
		else if (*fmt == 's') {
			s = va_arg(args, char *);
			std::cerr << s;
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

std::string extract_first_word(std::string & s, char sep)
{
	size_t cmd_end = s.find(sep);
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

void handler(int x)
{
	(void)x;
	std::cout
		<< C_RED "\n*** SIGINT has been caught, the server will now stop." C_R
		<< std::endl;
}