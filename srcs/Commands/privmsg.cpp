#include "../../includes/Server.hpp"

/* ERR_NOSUCHNICK (401)
ERR_NOSUCHSERVER (402)
ERR_CANNOTSENDTOCHAN (404)
ERR_TOOMANYTARGETS (407)
ERR_NORECIPIENT (411)
ERR_NOTEXTTOSEND (412)
ERR_NOTOPLEVEL (413)
ERR_WILDTOPLEVEL (414)
 */

void	Server::privMsg(std::vector<std::string> &cmds, Client *client, const std::string cmdLine) {
	if (cmds.size() < 3) {
		sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
		return;
	}
	std::vector<std::string> dest = splitString(cmds[1]);
	std::string msg = extractMessage(cmdLine, 2);
	for(std::vector<std::string>::iterator it = dest.begin(); it != dest.end(); it++) {
		if (it->at(0) == '#') {
			Channel *channelDest = getChannelByName(*it); // verificar se o nome do canal esta sendo armazenado com '#'
			if (channelDest && client->isChannelMember(channelDest)) {
				client->sendToChannel(channelDest, RPL_PRIVMSG(client->getNick(), channelDest->getName(), msg));
				//sendResponse(client->getSocket(), RPL_PRIVMSG(client->getNick(), channelDest->getName(), msg));
			}
			else if (channelDest && !client->isChannelMember(channelDest))
				sendResponse(client->getSocket(), ERR_CANNOTSENDTOCHAN(client->getNick(), channelDest->getName()));
			

		}	
		else {
			Client *clientDest = this->getClientByNick(*it);
			if (clientDest)
				sendResponse(clientDest->getSocket(), RPL_PRIVMSG(client->getNick(), clientDest->getNick(), msg));
			else 
				sendResponse(clientDest->getSocket(), ERR_NOSUCHNICK(client->getNick(), *it));
		}
	}

}