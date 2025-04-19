#include "../../includes/Server.hpp"

	/* ERR_NEEDMOREPARAMS (461) 
	ERR_BANNEDFROMCHAN (474)
	ERR_INVITEONLYCHAN (473)
	ERR_BADCHANNELKEY (475)
	ERR_CHANNELISFULL (471)
	ERR_BADCHANMASK (476)
	ERR_NOSUCHCHANNEL (403)
	ERR_TOOMANYCHANNELS (405)*/

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
	//colocar dentro de um loop
	std::map<std::string, std::string> mapChannelKey = parseJoinArgs(cmds);
	std::map<std::string, Channel *>::iterator itC;
	for(std::map<std::string, std::string>::iterator itM = mapChannelKey.begin(); itM != mapChannelKey.end(); itM++) {
		itC = this->_channels.find(itM->first);
		if (itC == _channels.end()) {
			if (itM->first[0] != '#')
				client->sendError(client, ERR_BADCHANMASK);
			else {
				Channel *newChannel = client->createChannel(itM->first, itM->second);
				this->_channels.insert(std::make_pair(newChannel->getName(), newChannel));
				std::string msg = ":" + client->getNick() + " JOIN " + newChannel->getName() + "\n";
				client->sendToClient(client, msg);
			}
		}
		else {
			Channel *channel = itC->second;
			std::string key = itM->second;
			//checar limite de users do canal
			//checar se o canal eh modo invite-only
			int ret = client->joinChannel(channel, key);
			//mensagens de erro serao tratadas aqui, dependendo do retorno de joinChannel
			(void) ret;


		}

	}
	
}

int	Client::joinChannel(Channel *channel, const std::string &key) {
	//SE O CLIENTE JA FIZER PARTE DO CANAL, NADA ACONTECE
	if (!isChannelMember(channel)) {
		if (key == channel->getKey()) {
			channel->addClient(this);
			//MENSAGEM DE BOAS VINDAS
			std::string msg = ":" + this->getNick() + " JOIN " + channel->getName() + "\n";
			//std::string msg = "Welcome to the channel " + channel->getName() + "\nTopic: " + channel->getTopic() + "\n"; 
			this->sendToClient(this, msg);
			//msg = "@" + this->getNick() + " joined the channel " + channel->getName() + "\n";
			this->sendToChannel(channel, msg);
			return 1;
		}
		else {
			this->sendToClient(this, ERR_BADCHANNELKEY(this->getNick(), channel->getName()));
		}
	}
	//desenvolver outras possibilidades, como:
	//tentar entrar num canal que esta mode invite-only, sem convite;
	//verificar se canal tem password;
	//verificar se o canal tem users limit;
	return 0;
		
	

}