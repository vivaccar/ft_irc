#include "../../includes/Server.hpp"

bool	Server::nickColission(std::string &nick)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNick() == nick)
			return true;
	}
	return false;
}

bool	validNickname(std::string &nick)
{
    if (nick.empty() || nick.length() > 9)
        return false;

    if (!isalpha(nick[0]))
        return false;

    for (size_t i = 1; i < nick.length(); ++i) {
        char c = nick[i];
        if (!isalpha(c) && !isdigit(c) &&
            c != '-' && c != '[' && c != ']' &&
            c != '\\' && c != '`' && c != '^' && c != '{' && c != '}') {
            return false;
        }
    }
    return true;
}

void    Server::setNick(std::vector<std::string> &cmds, Client *client)
{
	if (!client->passInserted())
		return sendResponse(client->getSocket(), ERR_NOTREGISTERED(client->getNick()));
	if (nickColission(cmds[1]))
		return sendResponse(client->getSocket(), ERR_NICKNAMEINUSE(client->getNick(), cmds[1]));
	if (cmds.size() < 2)
		return sendResponse(client->getSocket(), ERR_NONICKNAMEGIVEN(client->getNick()));
	if (validNickname(cmds[1]))
	{
		if (cmds.size() > 1)
		{
			std::string response = ":" + client->getPrefix() + std::string(" NICK ") + cmds[1] + "\n";
			client->setNick(cmds[1]);
			if (client->isAuth())
				send(client->getSocket(), response.c_str(), response.size(), 0);
			if (!client->getUser().empty() && !client->isAuth())
			{
				sendResponse(client->getSocket(), RPL_WELCOME(client->getNick(), client->getPrefix()));
				log(client->getPrefix() + " is now authenticated");
				client->setAuth(true);
			}
			return ;
		}
	}
	else
		return sendResponse(client->getSocket(), ERR_ERRONEUSNICKNAME(client->getNick(), cmds[1]));
	sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
}