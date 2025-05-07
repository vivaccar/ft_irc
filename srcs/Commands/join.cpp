#include "../../includes/Server.hpp"

std::map<std::string, std::string> parseJoinArgs(std::vector<std::string> &cmds) {
	std::map<std::string, std::string> map;

	std::vector<std::string> channels = splitString(cmds[1]);
	std::vector<std::string> keys;
	if (cmds.size() > 2)
		keys = splitString(cmds[2]);
	size_t i = 0;
	while (i < channels.size()) {
		if (i >= keys.size())
			map.insert(std::make_pair(channels[i], ""));
		else 
			map.insert(std::make_pair(channels[i], keys[i]));
		i++;
	}
	return map;
}

void    Server::joinCommand(std::vector<std::string> &cmds, Client *client) {
	if (cmds.size() < 2)
		return(sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0])));
	std::map<std::string, std::string> mapChannelKey = parseJoinArgs(cmds);
	for(std::map<std::string, std::string>::iterator it = mapChannelKey.begin(); it != mapChannelKey.end(); it++) {
		Channel *channelFound = this->getChannelByName(it->first);
		if (!channelFound) {
			if (it->first[0] == '#') {
				std::cout << "entrou no if certo" << std::endl;
				Channel *newChannel = client->createChannel(it->first);
				this->_channels.insert(std::make_pair(newChannel->getName(), newChannel));
				sendResponse(client->getSocket(), RPL_JOIN(client->getNick(), newChannel->getName()));
				showNames(newChannel, client);
			}
			else
				sendResponse(client->getSocket(), ERR_BADCHANMASK(it->first));
		}
		else if (channelFound) {
			std::cout << "entrou no if errado" << std::endl;
			std::cout << "NOME DO CANAL ENCONTRADO NO JOIN: " << channelFound->getName() << std::endl;
			std::string key = it->second;
			if (!client->isChannelMember(channelFound)) {
				if (channelFound->getUserLimit() != -1 && (static_cast<int>(channelFound->getClients().size()) >= channelFound->getUserLimit()))
					sendResponse(client->getSocket(), ERR_CHANNELISFULL(client->getNick(), channelFound->getName()));
				if (channelFound->getInviteOnly()) {
					if (client->isChannelInvited(channelFound)) {
						channelFound->addClient(client);
						client->sendToAllChannel(channelFound, RPL_JOIN(client->getNick(), channelFound->getName()));
						showNames(channelFound, client);
						std::vector<int>::iterator it = std::find(channelFound->getChannelInvites().begin(), channelFound->getChannelInvites().end(), client->getSocket());
						channelFound->getChannelInvites().erase(it);
						//remover cliente do invited ou nao?
					}
					else
						sendResponse(client->getSocket(), ERR_INVITEONLYCHAN(client->getNick(), channelFound->getName()));
				}				
				else if (key == channelFound->getKey() || channelFound->getKey().empty()) {
					channelFound->addClient(client);
					client->sendToAllChannel(channelFound, RPL_JOIN(client->getNick(), channelFound->getName()));
					showNames(channelFound, client);
					if (!channelFound->getTopic(1).empty())
						sendResponse(client->getSocket(), RPL_TOPIC(client->getNick(), channelFound->getName(), channelFound->getTopic(0)));
				}
				else {
					sendResponse(client->getSocket(), ERR_BADCHANNELKEY(client->getNick(), channelFound->getName()));
				}
			}
		}
	}	
}

void	Server::showNames(Channel *channel, Client *client) {
	std::string names("");
	std::vector<int> clients = channel->getClients();
	for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
		Client *cl = this->getClientBySocket(*it);
		if (cl->isChannelAdmin(channel))
			names+="@";
		names+=cl->getNick();
		if (it!= clients.end() - 1)
			names += " ";
	}
	client->sendToAllChannel(channel, RPL_NAMREPLY(client->getNick(), channel->getName(), names));
}