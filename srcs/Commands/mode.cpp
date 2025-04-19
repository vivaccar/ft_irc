#include "../../includes/Server.hpp"

void    Server::mode(std::vector<std::string> &cmds, Client *client, std::string cmd)
{
    (void)cmd;
    if (cmds.size() < 3)
        return sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));
    Channel *channel = getChannelByName(cmds[1]);
    if (!channel)
        return sendResponse(client->getSocket(), ERR_NOSUCHCHANNEL(client->getNick(), cmds[1]));
    if (!client->isChannelMember(channel))
        return sendResponse(client->getSocket(), ERR_NOTONCHANNEL(client->getNick(), channel->getName()));
    if (!client->isChannelAdmin(channel))
        return sendResponse(client->getSocket(), ERR_CHANOPRIVSNEEDED(client->getNick(), channel->getName()));
}

//VERIFICAR SE TEM MAIS DE UM PARAMETRO;
//VERIFICAR SE O CANAL EXISTE;
//VERIFICAR SE O CLIENTE QUE ESTA TENTANDO EXECUTAR E UM MODERADOR
