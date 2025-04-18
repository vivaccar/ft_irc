#include "../../includes/Server.hpp"
#include "../../includes/tests.hpp"

//PROCURA O CANAL PELO NOME
static Channel * ReturnChannel(std::map<std::string, Channel*> channels, std::string channel_name, Client * client)
{
	std::cout << "ReturnChannel \n";
	std::map<std::string, Channel*>::iterator channel_found = channels.begin();
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it ++)
	{
		if (channel_name.compare(it->first) == 0)
			return (channel_found->second);
	}
	client->sendToClient(client, ERR_NOSUCHCHANNEL_KICK(client->getNick(), channel_name));
	return (NULL);
}

//PROCURA PELO NICK
static Client * ReturnClient(Client *client, std::map<int, Client*> clients, std::string target_user, std::string channel_name)
{
	std::cout << "ReturnClient \n";
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it ++)
	{
		if (it->second->getNick().compare(target_user) == 0)
			return (it->second);
	}
	client->sendToClient(client, ERR_USERNOTINCHANNEL(client->getNick(), target_user, channel_name));
	return (NULL);
}

//Aqui a intencao eh verificar se o cliente eh adm do channel
static bool	isClientOperator(Client *client, Channel *channel)
{
	std::cout << "isClientOperator \n";
	std::vector<int> admins = channel->getAdmins();
	if (std::find(admins.begin(), admins.end(), client->getSocket()) != admins.end())
		return (true);
	client->sendToClient(client, ERR_CHANOPRIVSNEEDED(client->getNick(), channel->getName()));
	return (false);
}

//Aqui a intencao eh procurar o user escrito pelo Operator e verificar se ele existe e esta no canal
static bool	isTargetUserOnChannel(Channel *channel, Client *target)
{	
	std::cout << "isTargetUserOnChannel \n";

	if (!channel || !target)
		return (false);

	std::vector<int> 	clients = channel->getClients();
	int					client_fd = target->getSocket();

	for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it ++)
	{
		if (*it == client_fd)
			return (true);
	}
	//MENSAGEM DE ERRO!
	return (false);
}

static void	BroadcastMsgKick(Channel *channel, const std::string &msg) {
	std::cout << "BroadcastMsgKick \n";
	std::vector<int> members = channel->getClients();
	for (std::vector<int>::iterator it = members.begin(); it != members.end(); it++) {
		send(*it, msg.c_str(), msg.size(), 0);
	}
}

static void removeUserFromChannel(Channel *channel, int target_fd, std::string target_name, Client *client)
{
	std::cout << "removeUserFromChannel \n";

	std::vector<int> &clients = channel->getClientsRef();
	for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it == target_fd)
		{
			BroadcastMsgKick(channel, KICK_MSG(client->getNick(), target_name, channel->getName()));
			clients.erase(it);
			break;
		}
	}
}

//FORMAT ---> KICK #channel targetUser [:reason])
int	Server::kickUser(std::vector<std::string> &cmds, Client *client)
{
	std::cout << "kickUser method \n";

	Channel *channel = ReturnChannel(this->_channels, cmds[1], client);
	Client	*target = ReturnClient(client, this->_clients, cmds[2], cmds[1]);

	if (cmds.size() < 3)
	{
		client->sendToClient(client, ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
		return (EXIT_FAILURE);
	}

	else if (cmds.size() == 3) //in case where there is no reason, this will be defined here.
		cmds.push_back("Default reason to kick someone\n");

	if (channel && target && isClientOperator(client, channel) && isTargetUserOnChannel(channel, target))
	{
		removeUserFromChannel(channel, target->getSocket(), target->getNick(), client);
		//if user is the last of the channel
			//delete channel??? maybe.
			//broadcast a message to the server
	}
	else
	{
		std::cout << "NAO ENTROU NO IF DE CANAL VALIDO\n";
	}

	return (EXIT_SUCCESS);
}
