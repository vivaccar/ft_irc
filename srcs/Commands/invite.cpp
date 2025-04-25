/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:06:24 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/25 14:25:19 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"

//Command: INVITE
// /invite <nicK> <#channel>

//Parameters: <nickname> <channel>
int		Server::inviteUser(std::vector<std::string> &cmds, Client *client)
{
	std::cout << "INVITE USER \n" << std::endl;
	std::string target_name = cmds[1];
	std::string channel_name = cmds[2];
	Client *target = ReturnClient(client, this->_clients, target_name, channel_name);
	Channel *channel = ReturnChannel(this->_channels, channel_name, client);

	if (!channel || !target || !isClientOnChannel(client, channel))
	{
		client->sendToClient(client, ERR_NOTONCHANNEL(target_name, channel_name));
		return (EXIT_FAILURE);
	}
	//If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.
	else if (isClientOnChannel(target, channel))
	{
		client->sendToClient(client, ERR_USERONCHANNEL(target_name, channel_name));
		return (EXIT_FAILURE);
	}
	
	std::cout << "MODE :" << channel->getMode() << std::endl;
	//Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric.
	//In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator
	//Aqui, devo verificar se o mode do canal eh -i, se for, apenas operator pode enviar invites...
	
	//When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message,
	//with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.
	std::string msg =  "You have been invited to " + channel_name + " by " + client->getNick() + "\r\n";
	target->sendToClient(target, msg);
	client->sendToClient(client, RPL_INVITING(target_name, channel_name, client->getNick()));
	return (EXIT_SUCCESS);
};
//std::vector<int>	_channelInvites;

//CHECAR USER LIMITS TAMBEM ANTES DE ADICIONAR USERS