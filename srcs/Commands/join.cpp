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
		client->sendToClient(client, ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
		return;
	}
	std::map<std::string, std::string> mapChannelKey = parseJoinArgs(cmds);
	std::map<std::string, std::string>::iterator it;
	for(it = mapChannelKey.begin(); it != mapChannelKey.end(); it++) {
		Channel *channelFound = this->getChannelByName(it->first);
		if (!channelFound) {
			if (it->first[0] != '#')
				client->sendToClient(client, ERR_BADCHANMASK(it->first));
			else {
				Channel *newChannel = client->createChannel(it->first, it->second);
				this->_channels.insert(std::make_pair(newChannel->getName(), newChannel));
				std::string msg = ":" + client->getNick() + " JOIN " + newChannel->getName() + "\n";
				client->sendToClient(client, msg);
			}
		}
		else {
			std::string key = it->second;
			//checar limite de users do canal
			//checar se o canal eh modo invite-only
			//client->joinChannel(channelFound, key); //channel is full, 
			if (!client->isChannelMember(channelFound)) {
				if (key == channelFound->getKey()) {
					channelFound->addClient(this);
					sendResponse(client->getSocket(), RPL_JOIN(this->getNick(), channelFound->getName()));
					client->sendToChannel(channelFound, RPL_JOIN(this->getNick(), channelFound->getName()));
					if (!channelFound->getTopic().empty())
						this->sendToClient(this, RPL_TOPIC(channelFound->getName(), channelFound->getTopic()));
					return 1;
				}
				else {
					this->sendToClient(this, ERR_BADCHANNELKEY(this->getNick(), channelFound->getName()));
				}
			}
			//mensagens de erro serao tratadas aqui, dependendo do retorno de joinChannel

				//ESTAVA ARRUMANDO ESTA FUNCAO PARA RETIRAR A SEND TO CLIENT E SEND TO CHANNEL DA CLASSE "CLIENT"; ESTA MEIO CAMINHO ANDADO
		}

	}
	
}

int	Client::joinChannel(Channel *channel, const std::string &key) {
	//SE O CLIENTE JA FIZER PARTE DO CANAL, NADA ACONTECE
	//desenvolver outras possibilidades, como:
	//tentar entrar num canal que esta mode invite-only, sem convite;
	//verificar se canal tem password;
	//verificar se o canal tem users limit;
	return 0;
		
	

}