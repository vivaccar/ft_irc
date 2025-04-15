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
	if (!client->insertPassword())
	{
		send(client->getSocket(), ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
		return ;
	}
	if (nickColission(cmds[1]))
	{
		send(client->getSocket(), ": 433 ", strlen(": 433 "), 0);
		send(client->getSocket(), client->getNick().c_str(), strlen(client->getNick().c_str()), 0);
		send(client->getSocket(), " ", strlen(" "), 0);
		send(client->getSocket(), cmds[1].c_str(), strlen(cmds[1].c_str()), 0);
		send(client->getSocket(), ERR_NICKNAMEINUSE, strlen(ERR_NICKNAMEINUSE), 0);
		return ;
	}
	if (validNickname(cmds[1]))
	{
		if (cmds.size() > 1) 
		{
			std::string response = ":" + client->getNick() + std::string(" NICK ") + cmds[1] + "\n";
			client->setNick(cmds[1]);
			std::cout << "Client " << client->getSocket() << " :" << " set new NICKNAME :" << client->getNick() << std::endl;
			send(client->getSocket(), response.c_str(), response.size(), 0);
			return ;
		}
	}
	else
	{
		send(client->getSocket(), ERR_ERRONEUSNICKNAME, strlen(ERR_ERRONEUSNICKNAME), 0);
		return;
	}
	send(client->getSocket(), ERR_NEEDMOREPARAMS, strlen(ERR_NEEDMOREPARAMS), 0);
}