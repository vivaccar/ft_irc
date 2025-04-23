// - - - - - - ERRORS - - - - - - 
#define ERR_NEEDMOREPARAMS(nick, cmd) ": 461 " + nick + " " + cmd + " :Need more parameters\r\n"
#define ERR_PASSWDMISMATCH(nick) ": 464 " + nick + " :Invalid password\r\n"
#define ERR_ALREADYREGISTERED(nick) ": 462 " + nick + " :You may not reregister\r\n"
#define ERR_NOTREGISTERED(nick) ": 451 " + nick + " :You have not Registered\r\n"
#define ERR_NONICKNAMEGIVEN(nick) ": 431 " + nick + " :No Nickname Given\r\n"
#define ERR_NICKNAMEINUSE(nick, attempt) ": 433 " + nick + " " + attempt + " :Nickname in use.\r\n"
#define ERR_ERRONEUSNICKNAME(nick, attempt) ": 432 " + nick + " " + attempt + ":Erroneus nickname\r\n"
#define ERR_UNKNOWNCOMMAND(nick, cmd) ": 421 " + nick + " " + cmd + " :Unknown command\r\n"

#define ERR_BANNEDFROMCHAN(nick) ": 474 " + nick + " :You are banned from this channel\n"
#define ERR_INVITEONLYCHAN (nick, channel) ": 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(client, channel) ": 476 " + client + " " + channel + " :Cannot join channel (+k)\r\n"
#define ERR_CHANNELISFULL(nick, channel) ": 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n"
#define ERR_BADCHANMASK(channel) ": 476 " + channel + " :Bad Channel Mask\n"
#define ERR_TOOMANYCHANNELS "405 ERROR: You have joined too many channels\n"
#define ERR_NOSUCHCHANNEL(nick, channel) ": 403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_NOTONCHANNEL(nick, channel) ": 442 " + nick + " " + channel + " :You're not in the channel\r\n"
#define ERR_CANNOTSENDTOCHAN(nick, channel) ": 404 " + nick + " " + channel + " :Cannot send to channel\r\n"
#define ERR_NOTEXTTOSEND(nick) ": 412 " + nick + " :No text to send\r\n"

//KICK DEFINITIONS
#define ERR_NOSUCHNICK(nick, noexist) ": 401 " + nick + " " + noexist + " :No such nick\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel_name) ": 482 " + nick + " " + channel_name + " : You're not channel operator\r\n"
#define ERR_USERNOTINCHANNEL(nick, target, channel_name) ": 441 " + nick + " "  + target + " " + channel_name + " :They aren't on that channel"

//// - - - - - - REPLIES - - - - - - 
#define RPL_WELCOME(nick) ": 001 " + nick + " :Welcome " + nick + " to the ft_irc\r\n"
#define RPL_TOPIC(nick, channel ,topic) ": 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_NOTOPIC(nick, channel) ": 331 " + nick + " " + channel + " :No topic is set\r\n" 
#define KICK_MSG(nick, channel_name, target, reason) ":"+ nick + " KICK " + channel_name + " " + target + reason + "\r\n"
#define RPL_JOIN(nick, channel) ":" + nick + " JOIN " + channel + "\r\n"
#define RPL_PRIVMSG(nick, target, msg) ":" + nick + " PRIVMSG " + target + " :" + msg + "\r\n"

