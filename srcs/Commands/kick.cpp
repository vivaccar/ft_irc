/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfassbin <mfassbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:06:34 by aconceic          #+#    #+#             */
/*   Updated: 2025/05/17 17:29:00 by mfassbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"

static bool		isTargetUserOnChannel(Channel *channel, Client *target, Client *client);

//FORMAT OF MSG ---> KICK #channel targetUser [:reason])
//FORMAT OF MSG CAN BE ---> KICK #channel targetUser,targetUser2,targetUser3 [:reason])
void	Server::kickUser(std::vector<std::string> &cmds, Client *client)
{
	if (cmds.size() < 3)
		return(sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0])));
	else if (cmds.size() == 3)
		cmds.push_back("Default reason to kick someone");

	Channel *channel = getChannelByName(cmds[1]);
	std::vector<std::string> targets = splitVectorString(cmds);
	if (channel && client->isChannelAdmin(channel))
	{
		for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it ++)
		{
			Client	*target = getClientByNick(*it);
			if (target && isTargetUserOnChannel(channel, target, client))
			{
				client->removeUserFromChannel(channel, target, client, cmds);
				if (channel->getClients().size() == 0) {
					std::map<std::string, Channel *> &map = getChannelsMap();
					std::map<std::string, Channel *>::iterator it = map.find(channel->getName());
					map.erase(it);
					delete channel;
				}	
			}
		}
	}
}

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

