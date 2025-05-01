/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:12:43 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/25 14:36:46 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/utils.hpp"

bool    onlyNumbers(const std::string &str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

//This one is declared on Server.hpp
std::string extractMessage(std::string cmd, int maxSpaces)
{
    int index = 0;
    int spaces = 0;

    while (std::isspace(cmd[index]))
        index++;
    while (spaces < maxSpaces) 
    {
        if (std::isspace(cmd[index]))
        {
            while (std::isspace(cmd[index]))
                index++;
            spaces++;
        }
        else 
            index++;
    }
    return cmd.substr(index + 1, cmd.size());
}


/**********************************************/
/*              AMAURI DECLARATIONS           */
/**********************************************/
//USED ON kick.cpp
//Search the channel by name and returns it.
//returns NULL if there is no channel and send msg to client
Channel * ReturnChannel(std::map<std::string, Channel*> &channels, std::string channel_name, Client * client)
{
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it ++)
	{
		if (channel_name.compare(it->first) == 0)
			return (it->second);
	}
	client->sendToClient(client, ERR_NOSUCHCHANNEL(client->getNick(), channel_name));
	return (NULL);
}

//USED ON kick.cpp
//Searchs the cliente by the name and returns it.
//Returns null if there is no client.
Client * ReturnClient(Client *client, std::map<int, Client*> &clients, std::string target_user, std::string channel_name)
{
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it ++)
	{
		if (it->second->getNick().compare(target_user) == 0)
			return (it->second);
	}
	//client->sendToClient(client, ERR_NOSUCHNICK(client->getNick(), channel_name));
	(void)client;
	(void)channel_name;
	return (NULL);
}

//USED ON kick.cpp
//Check if the client is operator of the specific channel.
bool	isClientOperator(Client *client, Channel *channel)
{
	std::vector<int> admins = channel->getAdmins();
	if (std::find(admins.begin(), admins.end(), client->getSocket()) != admins.end())
		return (true);
	client->sendToClient(client, ERR_CHANOPRIVSNEEDED(client->getNick(), channel->getName()));
	//(void)client;
	return (false);
}

//used on invite.cpp
//Check on the list of all clients of a channel if a target is there.
//if not, send msg to target client saying that is not on channel.
bool	isClientOnChannel(Client *client, Channel *channel)
{
	if (!client || !channel)
		return (false);

	int	client_fd = client->getSocket();
	std::vector<int> clients_on_channel = channel->getClients();

	for (std::vector<int>::iterator it = clients_on_channel.begin(); 
			it != clients_on_channel.end(); it ++)
	{
		if ((*it) == client_fd)
			return (true);
	}
	return (false);
}

//used on kick.cpp
//Broadcast MSG to all the clients in the channel
void	BroadcastMsgToChannel(Channel *channel, const std::string &msg) 
{
	std::vector<int> &members = channel->getClientsRef();
	for (std::vector<int>::iterator it = members.begin(); it != members.end(); it++) {
		send(*it, msg.c_str(), msg.size(), 0);
	}
}

//used on kick.cpp
//Split a vector of strings using a "," as separator.
//Return a vector with all the strings.
std::vector<std::string> splitVectorString(const std::vector<std::string>& strs)
{
    std::vector<std::string> client_users;

    for (std::vector<std::string>::const_iterator it = strs.begin(); it != strs.end(); ++it)
    {
        const std::string& s = *it;
        std::string::size_type start = 0;

        while (true)
        {
            std::string::size_type end = s.find(',', start);
            client_users.push_back(s.substr(start, end - start));

            if (end == std::string::npos)
                break;

            start = end + 1;
        }
    }
    return (client_users);
}

//used on kick.cpp
//Remove user from channel, having in consideration the reason msg.
void removeUserFromChannel(Channel *channel, Client *target, Client *client, std::vector<std::string> &cmds)
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
			BroadcastMsgToChannel(channel, KICK_MSG(client->getNick(), channel->getName(), target_name, reason_msg));
			clients.erase(it);
			break;
		}
	}
}

std::vector<std::string> splitString(std::string &str) {
	size_t start = 0;
	size_t end = 0;
	std::vector<std::string> vec;
	while(end != str.size()) {
		end = str.find(",", start);
		if (end == str.npos)
			end = str.size();
		vec.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	return vec;
}