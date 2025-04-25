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
#define ERR_INVITEONLYCHAN ": 473 :Invite-only channel\n"
#define ERR_BADCHANNELKEY(client, channel) ": 476 " + client + " " + channel + " :Cannot join channel\n"
#define ERR_CHANNELISFULL "471 ERROR: Channel is full\n"
#define ERR_BADCHANMASK "476 ERROR: Bad Channel Mask\n"
#define ERR_TOOMANYCHANNELS "405 ERROR: You have joined too many channels\n"
#define ERR_NOSUCHCHANNEL(nick, channel) ": 403 " + nick + " " + channel + " :No such Channel\r\n"
#define ERR_NOTONCHANNEL(nick, channel) ": 442 " + nick + " " + channel + " :You're not in the channel\r\n"
#define ERR_INVALIDEMODEPARAM(nick, channel, mode) ": 696 " + nick + " " + channel + " :" + msg + "\r\n"
#define ERR_USERONCHANNEL(nick, channel_name) ": 443 " + nick + " " + channel_name + " :is already on channel\r\n"
#define ERR_NOSUCHNICK(nick, channel_name) ": 401 " + nick + " " + channel_name + " : No such channel\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel_name) ": 482 " + nick + " " + channel_name + " : You're not channel operator\r\n"
#define ERR_USERNOTINCHANNEL(nick, target, channel_name) ": 441 " + nick + " "  + target + " " + channel_name + " :They aren't on that channel"

//// - - - - - - REPLIES - - - - - - 
#define RPL_WELCOME(nick) ": 001 " + nick + " :Welcome " + nick + " to the ft_irc\r\n"
#define RPL_TOPIC(nick, channel ,topic) ": 332 " + nick + " " + channel + " " + topic + "\r\n"
#define RPL_NOTOPIC(nick, channel) ": 331 " + nick + " " + channel + " :No topic is set\r\n" 
#define KICK_MSG(nick, channel_name, target, reason) ":"+ nick + " KICK " + channel_name + " " + target + reason + "\r\n"
#define TOPIC_CHANGE(nick, channel, topic) ":" + nick + " TOPIC " + channel + " " + topic + "\r\n"
#define RPL_INVITING(target, channel_name, client) ": 341 " + client + " " + target + " " + channel_name + " : \r\n" //TALVEZ PRECISE DE MSG (AMAURI)

#define MODE(nick, channel, status) ":" + nick + " MODE " + channel + " :" + status + "\r\n"
#define SET_KEY(nick, channel, cmd, key) ":" + nick + " MODE " + channel + " " + cmd + " :" + key + "\r\n"
#define MODE_OPERATOR(nick, channel, cmd, target) ":" + nick + " MODE " + channel + " " + cmd + " :" + target + "\r\n"
//MODE #42 +o :vinicius
