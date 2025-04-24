/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:06:34 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/24 13:22:01 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"

//AUXILIARS FUNCTIONS DECLARATION

static bool		isTargetUserOnChannel(Channel *channel, Client *target, Client *client);

/**************************************/
/*             MAIN FUNTION           */
/**************************************/
//FORMAT OF MSG ---> KICK #channel targetUser [:reason])
//FORMAT OF MSG CAN BE ---> KICK #channel targetUser,targetUser2,targetUser3 [:reason])
//MAIN FUNCTION
int	Server::kickUser(std::vector<std::string> &cmds, Client *client)
{

	if (cmds.size() < 3)
	{
		client->sendToClient(client, ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
		return (EXIT_FAILURE);
	}
	else if (cmds.size() == 3)
		cmds.push_back("Default reason to kick someone\n");

	Channel *channel = ReturnChannel(this->_channels, cmds[1], client);
	std::vector<std::string> targets = splitVectorString(cmds);
	if (channel && isClientOperator(client, channel))
	{
		for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it ++)
		{
			Client	*target = ReturnClient(client, this->_clients, *it, cmds[1]);
			if (target && isTargetUserOnChannel(channel, target, client))
			{
				removeUserFromChannel(channel, target, client, cmds);
				//if user is the last of the channel
					//delete channel??? maybe.
					//broadcast a message to the server
			}
		}
	}
	
	return (EXIT_SUCCESS);
}

/**************************************/
/*              AUXILIARS             */
/**************************************/

//Check if target user is on the channel
//send error msg if not.
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



