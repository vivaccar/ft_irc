#include "../../includes/Server.hpp"

void    Server::setUser(std::vector<std::string> &cmds, Client *client)
{
	if (!client->passInserted())
		return sendResponse(client->getSocket(), ERR_NOTREGISTERED(client->getNick()));
	if (cmds.size() < 5)
		return sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
	if (client->isAuth() || !client->getUser().empty())
		return sendResponse(client->getSocket(), ERR_ALREADYREGISTERED(client->getNick()));
	for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); it++) {
		std::cout << *it << std::endl;
	}
	client->setUser(cmds[1]);
	std::string realName = cmds[4];
	if (cmds[4][0] == ':')
		realName = realName.substr(1, realName.size());
	if (cmds[4][0] == ':' && cmds.size() > 5)
		client->setRealName(realName + " " + cmds[5]);
	else
		client->setRealName(realName);
	if (!client->isAuth() && !client->getNick().empty())
	{
		sendResponse(client->getSocket(), RPL_WELCOME(client->getNick()));
		client->setAuth(true);
	}
	std::cout << "RealName is: " << client->getRealName() << std::endl;
}