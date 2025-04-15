#include "../../includes/Server.hpp"

void    Server::setNick(std::vector<std::string> &cmds, Client *client)
{
	if (!client->insertPassword())
	{
		send(client->getSocket(), ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
		return ;
	}
	if (cmds.size() > 1) {
		client->setNick(cmds[1]);
		std::cout << "Client " << client->getSocket() << " :" << " set new NICKNAME :" << client->getNick() << std::endl;
		std::string response = "New Nickname set! : " + client->getNick() + "\n";
		send(client->getSocket(), response.c_str(), response.size(), 0);
		return ;
	}
	send(client->getSocket(), ERR_NEEDMOREPARAMS, strlen(ERR_NEEDMOREPARAMS), 0);
}