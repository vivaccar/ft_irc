#include "../../includes/Server.hpp"

void    Server::topic(std::vector<std::string> &cmds, Client *client, std::string cmd)
{
    if (cmds.size() < 2)
        return sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getNick(), cmds[0]));
    
    Channel *channel = getChannelByName(cmds[1]);
    if (!channel)
        return sendResponse(client->getSocket(), ERR_NOSUCHCHANNEL(client->getNick(), cmds[1]));
    if (!client->isChannelMember(channel))
        return sendResponse(client->getSocket(), ERR_NOTONCHANNEL(client->getNick(), cmds[1]));
    if (cmds.size() == 2)
    {
        std::string topic = channel->getTopic();
        if (topic.empty())
            return sendResponse(client->getSocket(), RPL_NOTOPIC(client->getNick(), cmds[1]));    
        return sendResponse(client->getSocket(), RPL_TOPIC(client->getNick(), cmds[1], channel->getTopic()));
    }
    std::string newTopic = extractMessage(cmd, 2);
    std::cout << RED << "TOPIC IS --> " << newTopic << RESET << std::endl;
    channel->setTopic(newTopic);
    return;
}

// se tiver menos de 2 parametros --> NEEDMOREPARAMS.
// VERIFICAR SE O CANAL EXISTE. --> NOSUCHCHANNEL.
// Verificar se o cliente esta no canal, se nao estiver ->  ERR_NOTONCHANNEL (442).
// parametros TOPIC <channel> <topic> --> se o parametro topic nao for fornecido retornar o topic do canal RPL_TOPIC e depois RPL_TOPICWHOTIME
// Se <topic> for fornecido altera-se o topico do canal (caso permitido).


