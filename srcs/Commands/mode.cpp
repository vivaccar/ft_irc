#include "../../includes/Server.hpp"

void    inviteOnlyMode(std::string &cmd, Client *client, Channel *channel)
{
    if (cmd == "+i" && !channel->getInviteOnly())
    {
        channel->setInviteOnly(true);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "+i"));
    }
    else if (cmd == "-i" && channel->getInviteOnly())
    {
        channel->setInviteOnly(false);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "-i"));
        return;
    }
}

void    topicMode(std::string &cmd, Client *client, Channel *channel)
{
    if (cmd == "+t" && !channel->getTopicRestricted())
    {
        channel->setTopicRestricted(true);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "+t"));
    }
    else if (cmd == "-t" && channel->getTopicRestricted())
    {
        channel->setTopicRestricted(false);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "-t"));
    }
}

void    changePassword(std::string &cmd, std::vector<std::string>& cmds, Client *client, Channel *channel, unsigned int &parameter)
{
    if (cmd == "-k" && !channel->getKey().empty())
    {
        channel->setKey("");
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "-k"));
    }
    else if (cmd == "+k")
    {
        if (cmds.size() <= parameter)
        {
            std::string msg = "The <+k> mode needs a parameter <password>!";
            client->sendToClient(client, ERR_INVALIDEMODEPARAM(client->getNick(), channel->getName(), msg));
            return ;
        }
        channel->setKey(cmds[parameter]);
        client->sendToAllChannel(channel, SET_KEY(client->getNick(), channel->getName(), "+k", cmds[parameter]));
    }
}

bool    onlyNumbers(std::string &str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

void    changeUserLimit(std::string &cmd, std::vector<std::string>& cmds, Client *client, Channel *channel, unsigned int &parameter)
{
    if (cmd == "-l" && channel->getUserLimit() != -1)
    {
        channel->setUserLimit(-1);
        client->sendToAllChannel(channel, MODE(client->getNick(), channel->getName(), "-l"));
        return;
    }
    else if (cmd == "+l")
    {
        if (cmds.size() <= parameter)
        {            
            std::string msg = "The <+l> mode needs a parameter <number>";
            client->sendToClient(client, ERR_INVALIDEMODEPARAM(client->getNick(), channel->getName(), msg));
            return;
        }
        long limit = std::atol(cmds[parameter].c_str());
        if (limit < 1 || limit > std::numeric_limits<int>::max() || limit < std::numeric_limits<int>::min() || !onlyNumbers(cmds[parameter]))
        {
            std::string msg = "The <+l> parameter is invalid";
            client->sendToClient(client, ERR_INVALIDEMODEPARAM(client->getNick(), channel->getName(), msg));
            return;
        }
        else
        {
            channel->setUserLimit((int)limit);
            client->sendToAllChannel(channel, SET_KEY(client->getNick(), channel->getName(), "+l", cmds[parameter]));
        }
    }
}

void    executeModeCommands(std::string action, std::vector<std::string>& cmds, unsigned int &parameter, Client* client, Channel *channel)
{
    (void) parameter;
    (void) cmds;
    
    
    std::cout << action << " Size: " << action.size() <<  std::endl;
    if (action == "+i" || action == "-i")
        return inviteOnlyMode(action, client, channel);
    if (action == "+t" || action == "-t")
        return topicMode(action, client, channel);
    if (action == "+k" || action == "-k")
        return changePassword(action, cmds, client, channel, parameter);
    if (action == "+l" || action == "-l")
        return changeUserLimit(action, cmds, client, channel, parameter);
}

void parseModeCommands(std::vector<std::string>& cmds, Client* client, Channel *channel)
{
    std::string modes = cmds[2];
    char signal = '+';
    unsigned int curParameter = 3;

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
