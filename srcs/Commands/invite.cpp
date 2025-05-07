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
void	Server::inviteUser(std::vector<std::string> &cmds, Client *client)
{
	std::string target_name = cmds[1];
	std::string channel_name = cmds[2];
	Client *target = getClientByNick(target_name);
	Channel *channel = getChannelByName(channel_name);

	if (!channel || !target || !client->isChannelMember(channel))
		return sendResponse(client->getSocket(), ERR_NOTONCHANNEL(target_name, channel_name));
	else if (target->isChannelMember(channel))
		return sendResponse(client->getSocket(), ERR_USERONCHANNEL(target_name, channel_name));
	else if (channel->getInviteOnly() && !client->isChannelAdmin(channel))
		return sendResponse(client->getSocket(), ERR_CHANOPRIVSNEEDED(client->getNick(), channel_name));

	sendResponse(target->getSocket(), RPL_INVITED(channel_name, target_name));
	sendResponse(client->getSocket(), RPL_INVITING(target_name, channel_name, client->getNick()));
	channel->addChannelInvite(target);
}