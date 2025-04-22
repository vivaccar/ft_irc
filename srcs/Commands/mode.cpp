#include "../../includes/Server.hpp"

void    inviteOnlyMode(std::string &cmd, Client *client, Channel *channel)
{
    if (cmd == "i")
    {
        if (channel->getInviteOnly() == true)
            cmd = "-i";
        else
            cmd = "+i";
    }
    if (cmd == "+i")
    {
        channel->setInviteOnly(true);
        client->sendToChannel(channel, INVITE_ONLY(client->getNick(), channel->getName(), "+i"));
        return;
    }
    channel->setInviteOnly(false);
    client->sendToChannel(channel, INVITE_ONLY(client->getNick(), channel->getName(), "-i"));
}

void    topicMode(std::string &cmd, Client *client, Channel *channel)
{
    if (cmd == "t")
    {
        if (channel->getTopicRestricted() == true)
            cmd = "-t";
        else
            cmd = "+t";
    }
    if (cmd == "+t")
    {
        channel->setTopicRestricted(true);
        client->sendToChannel(channel, INVITE_ONLY(client->getNick(), channel->getName(), "+t"));
        return;
    }
    channel->setTopicRestricted(false);
    client->sendToChannel(channel, INVITE_ONLY(client->getNick(), channel->getName(), "-t"));
}

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
    if (cmds[2] == "+i" || cmds[2] == "-i" || cmds[2] == "i")
        return inviteOnlyMode(cmds[2], client, channel);
    if (cmds[2] == "+t" || cmds[2] == "-t" || cmds[2] == "t")
        return topicMode(cmds[2], client, channel);
    if (cmds[2] == "+k" || cmds[2] == "-k" || cmds[2] == "k")
        return topicMode(cmds[2], client, channel);
}

//VERIFICAR SE TEM MAIS DE UM PARAMETRO;
//VERIFICAR SE O CANAL EXISTE;
//VERIFICAR SE O CLIENTE QUE ESTA TENTANDO EXECUTAR E UM MODERADOR
