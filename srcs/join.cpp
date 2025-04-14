#include "../includes/Server.hpp"

	/* ERR_NEEDMOREPARAMS (461) 
	ERR_BANNEDFROMCHAN (474)
	ERR_INVITEONLYCHAN (473)
	ERR_BADCHANNELKEY (475)
	ERR_CHANNELISFULL (471)
	ERR_BADCHANMASK (476)
	ERR_NOSUCHCHANNEL (403)
	ERR_TOOMANYCHANNELS (405)*/

void    Server::joinCommand(std::vector<std::string> &cmds, Client *client) {
	std::map<std::string, Channel *>::iterator it = this->_channels.find(cmds[1]);
	if (it == _channels.end()) {
		if (cmds[1][0] != '#')
			client->sendError(client, ERR_BADCHANMASK);
		else {
			Channel *newChannel = client->createChannel(cmds[1]);
			this->_channels.insert(std::make_pair(newChannel->getName(), newChannel));
			std::string msg = "Channel " + newChannel->getName() + " has been created and @" + client->getNick() + " is the operator \n"; 
			client->sendToClient(client, msg);
		}
	}
	else {
		Channel *channel = it->second;
		//checar limite de users do canal
		//checar se o canal eh modo invite-only
		int ret = client->joinChannel(it->second);
		//mensagens de erro serao tratadas aqui, dependendo do retorno de joinChannel
		(void) ret;


	}
	
}

int	Client::joinChannel(Channel *channel) {
	//SE O CLIENTE JA FIZER PARTE DO CANAL, NADA ACONTECE
	if (!isChannelMember(channel)) {
		channel->addClient(this);
		//MENSAGEM DE BOAS VINDAS
		std::string msg = "Welcome to the channel " + channel->getName() + "\nTopic: " + channel->getTopic() + "\n"; 
		this->sendToClient(this, msg);
		msg = "@" + this->getNick() + " joined the channel " + channel->getName() + "\n";
		this->sendToChannel(channel, msg);
		return 1;
	}
	//desenvolver outras possibilidades, como:
	//tentar entrar num canal que esta mode invite-only, sem convite;
	//verificar se canal tem password;
	//verificar se o canal tem users limit;
	return 0;
		
	

}