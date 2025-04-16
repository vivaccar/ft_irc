#include "../../includes/Server.hpp"

void    Server::checkPassword(std::vector<std::string> &cmds, Client *client)
{
    if (client->passInserted())
        return sendResponse(client->getSocket(), ERR_ALREADYREGISTERED(client->getNick()));
    if (cmds.size() == 2)
    {
        if (cmds[1] == _password)
        {
            std::cout << "Client " << client->getSocket() << " type the correct password!" << std::endl;
            client->setInsertPassword(true);
        }
        else
        {
            std::cout << "Client " << client->getSocket() << " type the incorrect password!" << std::endl;
            sendResponse(client->getSocket(), ERR_PASSWDMISMATCH(client->getNick()));
        }
        return;
    }
    sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
}