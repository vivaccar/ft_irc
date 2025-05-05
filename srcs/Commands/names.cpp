#include "../../includes/Server.hpp"

#define RPL_NAMREPLY(client, channel, names) ": 353 " + client + " = " + channel + " :" + names + "\r\n"
#define RPL_ENDOFNAMES(client, channel) ": 366 " + client + " " + channel + " :End of /NAMES list\r\n"

std::string getListOfNames(Server *server, Channel *channel) {
	std::string names("");
	std::vector<int> clients = channel->getClients();
	for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
		Client *cl = server->getClientBySocket(*it);
		if (cl->isChannelAdmin(channel))
			names+="@";
		names+=cl->getNick();
		if (it!= clients.end() - 1)
			names += " ";
	}
	return names;
}

void	Server::namesCommand(Channel *channel, Client *client) {
	std::string names = getListOfNames(this, channel);
	client->sendToAllChannel(channel, RPL_NAMREPLY(client->getNick(), channel->getName(), names));
	//client->sendToAllChannel(channel, RPL_ENDOFNAMES(client->getNick(), channel->getName()));
}