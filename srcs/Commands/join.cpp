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
	if (cmds.size() < 2)
		return(sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0])));
	std::map<std::string, std::string> mapChannelKey = parseJoinArgs(cmds);
	for(std::map<std::string, std::string>::iterator it = mapChannelKey.begin(); it != mapChannelKey.end(); it++) {
		Channel *channelFound = this->getChannelByName(it->first);
		if (!channelFound) {
			if (it->first[0] == '#') {
				Channel *newChannel = client->createChannel(it->first);
				this->_channels.insert(std::make_pair(newChannel->getName(), newChannel));
				sendResponse(client->getSocket(), RPL_JOIN(client->getNick(), newChannel->getName()));
				namesCommand(newChannel, client);
			}
			else
				sendResponse(client->getSocket(), ERR_BADCHANMASK(it->first));
		}
		else if (channelFound) {
			std::string key = it->second;
			if (!client->isChannelMember(channelFound)) {
				if (channelFound->getUserLimit() != -1 && (static_cast<int>(channelFound->getClients().size()) >= channelFound->getUserLimit()))
					sendResponse(client->getSocket(), ERR_CHANNELISFULL(client->getNick(), channelFound->getName()));
				if (channelFound->getInviteOnly()) {
					if (client->isChannelInvited(channelFound)) {
						channelFound->addClient(client);
						client->sendToAllChannel(channelFound, RPL_JOIN(client->getNick(), channelFound->getName()));
						namesCommand(channelFound, client);
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
					namesCommand(channelFound, client);
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
