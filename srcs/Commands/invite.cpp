/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:06:24 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/24 12:59:02 by aconceic         ###   ########.fr       */
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

	//Aqui, devo verificar se o Client eh operator????
	/* || !isClientOperator(client, channel) */
	//Apenas operators podem mandar invite ou todos os membros??
	//Explicacao muito aberta no horse...
	if (!channel || !target)
		return (EXIT_FAILURE);
	

	//Only members of the channel are allowed to invite other users. 
	//Otherwise, the server MUST reject the command with the ERR_NOTONCHANNEL numeric.

	//Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric.
	//In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator

	//If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.
	
	//When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, 
	//with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.
	
	return (EXIT_SUCCESS);
};
