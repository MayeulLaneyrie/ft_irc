# FT_IRC

## by mlaneyri & shamizi

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
We based on the doc found at <modern.ircdocs.horse>

## Usage

`make` to compile. The server may then be launched with `./ircserv <port> <password>`.

here is a basic demonstration of ft_irc usage:
https://private-user-images.githubusercontent.com/45743413/446750740-5c29015e-7b8f-4544-bd41-a759649c5b83.mp4?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NDc5NDYxNjYsIm5iZiI6MTc0Nzk0NTg2NiwicGF0aCI6Ii80NTc0MzQxMy80NDY3NTA3NDAtNWMyOTAxNWUtN2I4Zi00NTQ0LWJkNDEtYTc1OTY0OWM1YjgzLm1wND9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNTA1MjIlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjUwNTIyVDIwMzEwNlomWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTE5ZTdiYTEyMTI1ZWY1NjFhY2NmYjdiNzg3ZDQ1OGI2M2QzZDU3OGMzYzcxZmY1MDhiYTUzZmNiZjgyMDJmZTEmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0In0.V-RjK1sMsQbXWxqvmTSgWIQbrhav0PZ3AYlnVMxvTL0

## Bot

A tiny bot is present, that may be compiled with `make bot`, and then launched
using `./bot <port> <nickname> <password>`. You may discover its functionalities
by sending him `PRIVMSG <bot nick> help`.

