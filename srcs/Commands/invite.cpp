/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:06:24 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/24 15:16:48 by aconceic         ###   ########.fr       */
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

	//debug
	//aqui verificar o nome do target
	//std::cout << "TARGET NAME : " << target_name << std::endl;
	//aqui verificar o channel do target
	//std::cout << "channel_name NAME : " << channel_name << std::endl;

	
	Client *target = ReturnClient(client, this->_clients, target_name, channel_name);
	//debug
	/* if (!target)
		std::cout << "SEM TARGET" << std::endl; */
	Channel *channel = ReturnChannel(this->_channels, channel_name, client);
	//debug
	/* if (!channel)
		std::cout << "SEM CHANNEL" << std::endl; */
	
	
	//Aqui, devo verificar se o Client eh operator????
	/* || !isClientOperator(client, channel) */
	//Apenas operators podem mandar invite ou todos os membros??
	//Explicacao muito aberta no horse...
	if (!channel || !target || !isClientOnChannel(client, channel, ERR_NOTONCHANNEL(target->getNick(), channel->getName())))
	{
		std::cout << "ENTROU INVALIDO" << std::endl;
		return (EXIT_FAILURE);
	}
	else if (isClientOnChannel(target, channel, ERR_USERONCHANNEL(target_name, channel_name))) //If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.
	{
		client->sendToClient(client, ERR_USERONCHANNEL(target_name, channel_name));
		return (EXIT_FAILURE);
	}
	
	std::cout << "MODE :" << channel->getMode() << std::endl;
	//Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric.
	//In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator
	
	
	//When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, 
	//with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.
	
	return (EXIT_SUCCESS);
};
