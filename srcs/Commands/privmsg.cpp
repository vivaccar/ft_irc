#include "../../includes/Server.hpp"

void	Server::privMsg(std::vector<std::string> &cmds, Client *client, const std::string cmdLine) {
	if (cmds.size() < 3) {
		if (cmds.size() == 2)
			return (sendResponse(client->getSocket(), ERR_NOTEXTTOSEND(client->getNick())));
		else
			return (sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0])));
	}
	std::vector<std::string> dest = splitString(cmds[1]);
	std::string msg = extractMessage(cmdLine, 2);
	for(std::vector<std::string>::iterator it = dest.begin(); it != dest.end(); it++) {
		if (it->at(0) == '#') {
			Channel *channelDest = getChannelByName(*it);
			if (channelDest && client->isChannelMember(channelDest))
				client->sendToChannel(channelDest, RPL_PRIVMSG(client->getNick(), channelDest->getName(), msg));
			else if (channelDest && !client->isChannelMember(channelDest))
				sendResponse(client->getSocket(), ERR_CANNOTSENDTOCHAN(client->getNick(), channelDest->getName()));
		}
		else {
			Client *clientDest = this->getClientByNick(*it);
			if (clientDest)
				sendResponse(clientDest->getSocket(), RPL_PRIVMSG(client->getNick(), clientDest->getNick(), msg));
			else  
				sendResponse(client->getSocket(), ERR_NOSUCHNICK(client->getNick(), *it));
		}
	}
}
