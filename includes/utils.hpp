/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:04:30 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/25 13:53:19 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include "../includes/Server.hpp"

/**************************************/
/*              TEMPLATES             */
/**************************************/
template <typename T>
void print_container(T cont, std::string msg)
{
	if (!msg.empty())
	{
		std::cout << msg << std::endl;
	}
	for (typename T::iterator it = cont.begin(); it != cont.end(); it ++)
	{
		std::cout << *it << std::endl;
	}
}

/**************************************/
/*              DECLARATIONS          */
/**************************************/

//std::string extractMessage(std::string cmd, int maxSpaces); this is declared on Served.hpp

Channel 					*ReturnChannel(std::map<std::string, Channel*> &channels, std::string channel_name, Client * client);
Client 						*ReturnClient(Client *client, std::map<int, Client*> &clients, std::string target_user, std::string channel_name);
bool						isClientOperator(Client *client, Channel *channel);
bool						isClientOnChannel(Client *target, Channel *channel);
void						BroadcastMsgToChannel(Channel *channel, const std::string &msg);
std::vector<std::string> 	splitVectorString(const std::vector<std::string>& strs);
void 						removeUserFromChannel(Channel *channel, Client *target, Client *client, std::vector<std::string> &cmds);


#endif