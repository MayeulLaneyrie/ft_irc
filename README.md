# FT_IRC

##by mlaneyri & shamizi

125/100 triple outstanding irc.

This is a minimalistic irc server coded for the 42 school cursus. It uses epoll
and implement most commands. However, multiserver is not managed.

The following commands are currently implemented:
```
 	- PASS <password>
 	- NICK <nickname>
 	- USER <username> <ignored> <ignored> <realname> 
 	- PING <token>
 	- QUIT [<reason>]
 	- PRIVMSG <target>{,<target>} <text to be sent>
 	- NOTICE <target>{,<target>} <text to be sent>
 	- OPER <name> <password>
 	- KILL <nickname> <comment>
 	- JOIN <channel>{,<channel>} [<key>{,<key>}]
 	- INVITE <nickname> <channel>
 	- TOPIC <channel> [<topic>]
 	- KICK <channel> <user> *( "," <user> ) [<comment>]
 	- WHO <mask>
 	- WHOIS [<target>] <nick>
 	- MODE <target> [<modestring> [<mode arguments>...]]
 	- PART <channel>{,<channel>} [<reason>]
 	- NAMES <channel>{,<channel>}
```
We based on the doc found at: modern.ircdocs.horse.

## Usage

`make` to compile. The server may then be launched with `./ircserv <port> <password>`.

## Bot

A tiny bot is present, that may be compiled with `make` bot, and then launched
using `./bot <port> <nickname> <password>`. You may discover its functionalities
by sending him `PRIVMSG <bot nick> help`.

