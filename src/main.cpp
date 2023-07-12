/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:16:34 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/07/12 17:19:21 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Server.hpp"

int main(int ac, char **av) {

	if (ac < 3)
		die("s", "Usage: ./ircserv <port> <password>");
	for (int i = 0; av[1][i]; ++i)
		if (av[1][i] < '0' || av[1][i] > '9' || i > 4)
			die("ssc", "Invalid port value : '", av[1], '\'');

	Server serv(atoi(av[1]), av[2]);
	return (serv.run());
}


