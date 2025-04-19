/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:06:24 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/19 15:15:17 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/tests.hpp"

//Command: INVITE
//Parameters: <nickname> <channel>
int		Server::inviteUser(std::vector<std::string> &cmds, Client *client)
{
	(void)cmds;
	(void)client;
	std::cout << "INVITE USER \n" << std::endl;
	

	return (EXIT_SUCCESS);
};
