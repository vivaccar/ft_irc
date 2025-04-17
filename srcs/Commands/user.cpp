#include "../../includes/Server.hpp"

void    Server::setUser(std::vector<std::string> &cmds, Client *client)
{
	if (!client->passInserted())
		return sendResponse(client->getSocket(), ERR_NOTREGISTERED(client->getNick()));
	if (cmds.size() < 5)
		return sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
	if (client->isAuth())
		return sendResponse(client->getSocket(), ERR_ALREADYREGISTERED(client->getNick()));
	client->setUser(cmds[1]);
	if (!client->isAuth() && !client->getNick().empty())
	{
		sendResponse(client->getSocket(), WELCOME(client->getNick()));
		client->setAuth(true);
	}
}