#include "../../includes/Server.hpp"

void    Server::setUser(std::vector<std::string> &cmds, Client *client)
{
	if (!client->insertPassword())
	{
		send(client->getSocket(), ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
		return;
	}
	if (client->getNick().empty())
	{
		send(client->getSocket(), ERR_NONICKNAMEGIVEN, strlen(ERR_NONICKNAMEGIVEN), 0);
		return;
	}
	client->setUser(cmds[1]);
	std::cout << "Client " << client->getSocket() << " :" << " set new USERNAME :" << client->getUser() << std::endl;
	const char* response = "Username set!\nNow you are able to join/create a channel\n";
	client->setAuth(true);
	send(client->getSocket(), response, strlen(response), 0);
}