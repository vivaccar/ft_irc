#include "../../includes/Server.hpp"

void	Server::who(std::vector<std::string> &cmds, Client *client) {
	if (cmds.size() == 1) 
		return (sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0])));
	Channel *channelFound = getChannelByName(cmds[1]);
	if (channelFound) {
		std::vector<int> clients = channelFound->getClients();
		for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
			Client *cl = getClientBySocket(*it);
			sendResponse(client->getSocket(), RPL_WHOREPLY(client->getNick(), channelFound->getName(), cl->getUser(), cl->getHostname(), cl->getNick(), cl->getRealName()));
		}
		sendResponse(client->getSocket(), RPL_ENDOFWHO(client->getNick(), channelFound->getName()));
	}	
}