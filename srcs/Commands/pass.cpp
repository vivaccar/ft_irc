#include "../../includes/Server.hpp"

void    Server::checkPassword(std::vector<std::string> &cmds, Client *client)
{
    if (client->passInserted())
    {
        send(client->getSocket(), ERR_ALREADYREGISTERED, strlen(ERR_ALREADYREGISTERED), 0);
        return;
    }
    if (cmds.size() == 2)
    {
        if (cmds[1] == _password)
        {
            std::cout << "Client " << client->getSocket() << " type the correct password!" << std::endl;
            client->setInsertPassword(true);
			const char *response = "Correct password! You are connected to the server\nNow insert your nickname\n";
            send(client->getSocket(), response, strlen(response), 0);
        }
        else
        {
            std::cout << "Client " << client->getSocket() << " type the incorrect password!" << std::endl;
            send(client->getSocket(), ERR_PASSWDMISMATCH, strlen(ERR_PASSWDMISMATCH), 0);
        }
        return;
    }
    send(client->getSocket(), ERR_NEEDMOREPARAMS, strlen(ERR_NEEDMOREPARAMS), 0);
}