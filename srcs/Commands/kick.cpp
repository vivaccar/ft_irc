/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:06:34 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/19 16:01:14 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/tests.hpp"

//AUXILIARS FUNCTIONS DECLARATION
static Channel 	*ReturnChannel(std::map<std::string, Channel*> &channels, std::string channel_name, Client * client);
static Client 	*ReturnClient(Client *client, std::map<int, Client*> &clients, std::string target_user, std::string channel_name);
static bool		isClientOperator(Client *client, Channel *channel);
static bool		isTargetUserOnChannel(Channel *channel, Client *target, Client *client);
static void		BroadcastMsgKick(Channel *channel, const std::string &msg);
static void 	removeUserFromChannel(Channel *channel, Client *target, Client *client, std::vector<std::string> &cmds);

/**************************************/
/*             MAIN FUNTION           */
/**************************************/
//FORMAT OF MSG ---> KICK #channel targetUser [:reason])
//FORMAT OF MSG CAN BE ---> KICK #channel targetUser,targetUser2,targetUser3 [:reason])
//MAIN FUNCTION
int	Server::kickUser(std::vector<std::string> &cmds, Client *client)
{
	std::cout << "kickUser method \n";

	if (cmds.size() < 3)
	{
		client->sendToClient(client, ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
		return (EXIT_FAILURE);
	}
	else if (cmds.size() == 3) //in case where there is no reason, this will be defined here.
		cmds.push_back("Default reason to kick someone\n");
	else if (cmds.size() > 3)
	{
		//separar users para kickalos de uma vez
		//eles sao separados por ,(virgula) sem espaco entre os nicks
	}

	Channel *channel = ReturnChannel(this->_channels, cmds[1], client);
	Client	*target = ReturnClient(client, this->_clients, cmds[2], cmds[1]);

	if (channel && target && isClientOperator(client, channel) && isTargetUserOnChannel(channel, target, client))
	{
		removeUserFromChannel(channel, target, client, cmds);
		//if user is the last of the channel
			//delete channel??? maybe.
			//broadcast a message to the server
	}
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


/**************************************/
/*              AUXILIARS             */
/**************************************/

//Search the channel by name and returns it.
//returns NULL if there is no channel
static Channel * ReturnChannel(std::map<std::string, Channel*> &channels, std::string channel_name, Client * client)
{

	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it ++)
	{
		if (channel_name.compare(it->first) == 0)
			return (it->second);
	}
	client->sendToClient(client, ERR_NOSUCHCHANNEL);
	return (NULL);
}

//Searchs the cliente by the name and returns it.
//Returns null if there is no cliente.
static Client * ReturnClient(Client *client, std::map<int, Client*> &clients, std::string target_user, std::string channel_name)
{

	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it ++)
	{
		if (it->second->getNick().compare(target_user) == 0)
			return (it->second);
	}
	client->sendToClient(client, ERR_NOSUCHNICK(client->getNick(), channel_name));
	return (NULL);
}

//Check if the cliente is operator of the specific channel.
static bool	isClientOperator(Client *client, Channel *channel)
{

	std::vector<int> admins = channel->getAdmins();
	if (std::find(admins.begin(), admins.end(), client->getSocket()) != admins.end())
		return (true);
	client->sendToClient(client, ERR_CHANOPRIVSNEEDED(client->getNick(), channel->getName()));
	return (false);
}

//Check if target user is on the channel
static bool	isTargetUserOnChannel(Channel *channel, Client *target, Client *client)
{	

	if (!channel || !target)
		return (false);

	std::vector<int> 	clients = channel->getClients();
	int					client_fd = target->getSocket();

	for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it ++)
	{
		if (*it == client_fd)
			return (true);
	}
	client->sendToClient(client, ERR_USERNOTINCHANNEL(client->getNick(), target->getNick(), channel->getName()));
	return (false);
}

//Broadcast MSG to all the clients in the channel
static void	BroadcastMsgKick(Channel *channel, const std::string &msg) {

	std::vector<int> &members = channel->getClientsRef();
	for (std::vector<int>::iterator it = members.begin(); it != members.end(); it++) {
		send(*it, msg.c_str(), msg.size(), 0);
	}
}

//Remove user from channel, having in consideration the reason msg.
static void removeUserFromChannel(Channel *channel, Client *target, Client *client, std::vector<std::string> &cmds)
{
	int target_fd = target->getSocket();
	std::string target_name = target->getNick();
	std::string	reason_msg;
	std::string tmp;

	for (std::vector<std::string>::iterator it = cmds.begin() + 3; it != cmds.end(); it ++)
	{
		tmp = reason_msg + " ";
		reason_msg = tmp + (*it);
	}
	reason_msg = reason_msg + "\n";

	std::vector<int> &clients = channel->getClientsRef();
	for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it == target_fd)
		{
			BroadcastMsgKick(channel, KICK_MSG(client->getNick(), channel->getName(), target_name, reason_msg));
			clients.erase(it);
			break;
		}
	}
}

