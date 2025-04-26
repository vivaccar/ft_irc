#include "../../includes/Server.hpp"

	/* ERR_NEEDMOREPARAMS (461) x
	ERR_BANNEDFROMCHAN (474)
	ERR_INVITEONLYCHAN (473)
	ERR_BADCHANNELKEY (475) x
	ERR_CHANNELISFULL (471)
	ERR_BADCHANMASK (476) x
	*/

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
	if (cmds.size() < 2) {
		sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
		return;
	}
	std::map<std::string, std::string> mapChannelKey = parseJoinArgs(cmds);
	for(std::map<std::string, std::string>::iterator it = mapChannelKey.begin(); it != mapChannelKey.end(); it++) {
		Channel *channelFound = this->getChannelByName(it->first);
		if (!channelFound) {
			if (it->first[0] == '#') {
				Channel *newChannel = client->createChannel(it->first);
				this->_channels.insert(std::make_pair(newChannel->getName(), newChannel));
				sendResponse(client->getSocket(), RPL_JOIN(client->getNick(), newChannel->getName()));
			}
			else
				sendResponse(client->getSocket(), ERR_BADCHANMASK(it->first));
		}
		else if (channelFound) {
			std::string key = it->second;
			//checar limite de users do canal -> ERR_CHANNELISFULL (471)
			//checar se o canal eh modo invite-only -> ERR_INVITEONLYCHAN (473)
			//channel is full, 
			if (!client->isChannelMember(channelFound)) {
				std::cout << " GET USER LIMIT -> " << channelFound->getUserLimit() << std::endl;
				std::cout << " GET CLIENTS SIZE -> " << channelFound->getClients().size() << std::endl;
				if (channelFound->getUserLimit() != -1 && (static_cast<int>(channelFound->getClients().size()) >= channelFound->getUserLimit()))
					sendResponse(client->getSocket(), ERR_CHANNELISFULL(client->getNick(), channelFound->getName()));
				if (channelFound->getInviteOnly()) {
					if (client->isChannelInvited(channelFound)) {
						channelFound->addClient(client);
						sendResponse(client->getSocket(), RPL_JOIN(client->getNick(), channelFound->getName()));
						//remover cliente do invited
					}
					else
						sendResponse(client->getSocket(), ERR_INVITEONLYCHAN(client->getNick(), channelFound->getName()));
				}				
				else if (key == channelFound->getKey() || channelFound->getKey().empty()) {
					channelFound->addClient(client);
					sendResponse(client->getSocket(), RPL_JOIN(client->getNick(), channelFound->getName()));
					client->sendToChannel(channelFound, RPL_JOIN(client->getNick(), channelFound->getName()));
					if (!channelFound->getTopic().empty())
						sendResponse(client->getSocket(), RPL_TOPIC(client->getNick(), channelFound->getName(), channelFound->getTopic()));
				}
				else {
					sendResponse(client->getSocket(), ERR_BADCHANNELKEY(client->getNick(), channelFound->getName()));
				}
			}
		}
	}	
}
