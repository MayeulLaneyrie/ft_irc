#include "User.hpp"

int User::_cmd_BOT(Msg & cmd) // --------------------------------------- BOT
{
	str_vec arg = cmd.payloadAsVector(2);
    str list_cmd = "/invite, /join, /kick, /kill, /mode, /nick, /pass, /topic, whois";
	if (arg.size() < 1)
		return (rpl(BOT_HELP, "use HELP, for a list of command or HELP <command> for information"));
    if ((arg[0] == "HELP" || arg[0] == "help") && arg.size() == 1)
        return(rpl(BOT_HELP, "@bot: list of command :" + list_cmd));
    else if ((arg[0] == "HELP" || arg[0] == "help") && arg.size() == 2)
    {
        if (arg[1] == "invite")
            return (rpl(BOT_HELP, "@bot: INVITE <nickname> <channel>: invite someone to a channel"));
        else if (arg[1] == "join")
            return (rpl(BOT_HELP, "@bot: JOIN <channel> [key]: join a channel, create it if it doesn't exist"));
        else if (arg[1] == "kick")
            return (rpl(BOT_HELP, "@bot: KICK <channel> <user> [comments]: kick a user from a channel with an optionnal reason"));
        else if (arg[1] == "kill")
            return (rpl(BOT_HELP, "@bot: KILL <nickname> <comments>: close connection from a user and the server"));
        else if (arg[1] == "mode")
            return (rpl(BOT_HELP, "@bot: MODE <channel> [mode string]: set mode of the channel"));
        else if (arg[1] == "nick")
            return (rpl(BOT_HELP, "@bot: NICK <nickname>: give/change the nickname"));
        else if (arg[1] == "pass")
            return (rpl(BOT_HELP, "@bot: PASS <password>: set a connection password"));
        else if (arg[1] == "topic")
            return (rpl(BOT_HELP, "@bot: TOPIC <channel> [topic]: show/set the topic of a channel"));
        else if (arg[1] == "whois")
            return (rpl(BOT_HELP, "@bot: WHOIS [target] <nick>: give query informatin about particular users"));  
    }
    else
    {
        return (rpl(BOT_HELP, "I don't understand what you're asking but i hope you enjoyed your day. Welcome to MAYEUL/SAID Internet Chat Relay"));
    }
    return (0);
}