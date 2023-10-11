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

str extract_first_word(str & s, char sep)
{
	size_t word_len = s.find(sep);
	if (word_len == str::npos)
		word_len = s.size();
	str ret = s.substr(0, word_len);
	s.erase(0, word_len);
	s.erase(0, s.find_first_not_of(sep));
	return (ret);
}

int sed(str & s, str const & from, str const & to)
{
	size_t pos = s.find(from);
	if (pos != str::npos) {
		s.erase(pos, from.size());
		s.insert(pos, to);
		return (1);
	}
	return (0);
}

void sighandler(int x)
{
	(void)x;
	std::cout
		<< C_RED "\n*** SIGINT has been caught, the server will now stop." C_R
		<< std::endl;
}

str mode_str(unsigned int bitset, const char * charset, std::map<char, str> vars)
{
	unsigned int i;
	unsigned int mask = 1;
	str char_str = "";
	str var_str = "";

	for (i = 0; charset[i] && i < 8 * sizeof(unsigned int) - 1; ++i) {
		if (bitset & mask) {
			char_str += charset[i];
			if (!vars.count(charset[i]))
				(var_str += ' ') += vars[charset[i]];
		}
		mask *= 2;
	}
	return (char_str + var_str);
}

str int_to_str(int n)
{
	std::ostringstream oss;

	oss << n;
	return (oss.str());
}