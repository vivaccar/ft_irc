/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:06:24 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/25 15:12:07 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"

//Command: INVITE
// /invite <nicK> <#channel>
//Parameters: <nickname> <channel>
int		Server::inviteUser(std::vector<std::string> &cmds, Client *client)
{
	std::string target_name = cmds[1];
	std::string channel_name = cmds[2];
	Client *target = ReturnClient(client, this->_clients, target_name, channel_name);
	Channel *channel = ReturnChannel(this->_channels, channel_name, client);

	if (!channel || !target || !isClientOnChannel(client, channel))
	{
		client->sendToClient(client, ERR_NOTONCHANNEL(target_name, channel_name));
		return (EXIT_FAILURE);
	}
	else if (isClientOnChannel(target, channel))
	{
		client->sendToClient(client, ERR_USERONCHANNEL(target_name, channel_name));
		return (EXIT_FAILURE);
	}
	else if (channel->getInviteOnly() && !isClientOperator(client, channel))
	{
		client->sendToClient(client, ERR_CHANOPRIVSNEEDED(client->getNick(), channel_name));
		return (EXIT_FAILURE);
	}

	std::string msg =  "You have been invited to " + channel_name + " by " + client->getNick() + "\r\n";
	target->sendToClient(target, msg);
	client->sendToClient(client, RPL_INVITING(target_name, channel_name, client->getNick()));
	channel->addChannelInvite(target);
	return (EXIT_SUCCESS);
};