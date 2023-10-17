#include "utils.hpp"
#include "network.hpp"
#include "Msg.hpp"
#include "Bot.hpp"
#include "namecheck.hpp"

int main(int ac, char **av)
{
	if (ac < 4)
		die("s", "Usage: ./bot <port> <nick> <password>");
	for (int i = 0; av[1][i]; ++i)
		if (av[1][i] < '0' || av[1][i] > '9' || i > 4)
			die("ss", "Invalid port value", av[1]);

	Bot bot(atoi(av[1]), av[2], av[3]);
	return (bot.run());
}