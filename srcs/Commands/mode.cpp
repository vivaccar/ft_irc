#include "../../includes/Server.hpp"

void    inviteOnlyMode(std::string &cmd, Client *client, Channel *channel)
{
    if (cmd == "+i" && !channel->getInviteOnly())
    {
        std::cout << "entrou no +i if" << std::endl;
        channel->setInviteOnly(true);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "+i"));
    }
    else if (cmd == "-i" && channel->getInviteOnly())
    {
        std::cout << "entrou no -i if" << std::endl;
        channel->setInviteOnly(false);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "-i"));
        return;
    }
}

void    topicMode(std::string &cmd, Client *client, Channel *channel)
{
    if (cmd == "+t" && !channel->getTopicRestricted())
    {
        std::cout << "entrou no +t if" << std::endl;
        channel->setTopicRestricted(true);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "+t"));
    }
    else if (cmd == "-t" && channel->getTopicRestricted())
    {
        std::cout << "entrou no -t if" << std::endl;
        channel->setTopicRestricted(false);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "-t"));
    }
}

/* void    changePassword(std::string &cmd, std::vector<std::string>& cmds, Client *client, Channel *channel, int &parameter)
{

    if (cmds.size() <= parameter)
    {
        //lancar erro
    }
    //LOGICA DE TROCA DE SENHA...
} */

void executeModeCommands(std::string action, std::vector<std::string>& cmds, int &parameter, Client* client, Channel *channel)
{
    (void) parameter;
    
    std::cout << action << std::endl;
    if (action == "+i" || action == "-i")
        return inviteOnlyMode(cmds[2], client, channel);
    if (action == "+t" || action == "-t")
        return topicMode(cmds[2], client, channel);
/*     if (action == "+k" || action == "-k")
        return changePassword(action, cmds,client, channel, parameter); */
}

void parseModeCommands(std::vector<std::string>& cmds, Client* client, Channel *channel)
{
    std::string modes = cmds[2];
    char signal = '+';
    int curParameter = 3;

    for (size_t i = 0; i < modes.size(); i++)
    {
        if (modes[i] == '+')
        {
            signal = '+';
            continue;
        }
        else if (modes[i] == '-')
        {
            signal = '-';
            continue;
        }

        if (modes[i] && modes[i] != '+' && modes[i] != '-')
        {
            std::string action = "";
            action += signal;
            action += modes[i];
            executeModeCommands(action, cmds, curParameter, client, channel);
            if (modes[i] == 'k' || modes[i] == 'l' || modes[i] == 'o')
                curParameter++;
        }
    }
}

// +ilt
// separa o mode em primeiro lugar da string e depois coloca o parametro nos modes caso necessario


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
    parseModeCommands(cmds, client, channel);
}

//VERIFICAR SE TEM MAIS DE UM PARAMETRO;
//VERIFICAR SE O CANAL EXISTE;
//VERIFICAR SE O CLIENTE QUE ESTA TENTANDO EXECUTAR E UM MODERADOR
