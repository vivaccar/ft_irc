#include "../../includes/Server.hpp"

std::string getTimestamp()
{
    std::time_t timestamp = std::time(NULL);

    std::stringstream str;
    str << timestamp;
    std::cout << timestamp << std::endl;
    return str.str();
}

void    Server::topic(std::vector<std::string> &cmds, Client *client, std::string cmd)
{
    if (cmds.size() < 2)
        return sendResponse(client->getSocket(), ERR_NEEDMOREPARAMS(client->getPrefix(), cmds[0]));
    
    Channel *channel = getChannelByName(cmds[1]);
    if (!channel)
        return sendResponse(client->getSocket(), ERR_NOSUCHCHANNEL(client->getPrefix(), cmds[1]));
    if (!client->isChannelMember(channel))
        return sendResponse(client->getSocket(), ERR_NOTONCHANNEL(client->getPrefix(), cmds[1]));
    if (cmds.size() == 2)
    {
        std::string topic = channel->getTopic(0);
        if (topic.empty())
            return sendResponse(client->getSocket(), RPL_NOTOPIC(client->getPrefix(), cmds[1])); 
        sendResponse(client->getSocket(), RPL_TOPIC(client->getPrefix(), cmds[1], channel->getTopic(0)));
        return sendResponse(client->getSocket(), RPL_TOPICWHOTIME(client->getNick(), cmds[1], channel->getTopic(1), channel->getTopic(2)));
    }
    std::string newTopic = extractMessage(cmd, 2);
    if (newTopic[0] == ':' && newTopic.size() == 3)
        newTopic = "";
    std::cout << RED << "TOPIC IS --> " << newTopic << " size: " << newTopic.size() << RESET << std::endl;
    channel->setTopic(newTopic, client->getPrefix(), getTimestamp());
    client->sendToAllChannel(channel, TOPIC_CHANGE(client->getPrefix(), channel->getName(), newTopic)); // MUDAR PARA A FUNCAO QUE MANDA PARA TODOS DO CANAL;
    return;
}

// se tiver menos de 2 parametros --> NEEDMOREPARAMS.
// VERIFICAR SE O CANAL EXISTE. --> NOSUCHCHANNEL.
// Verificar se o cliente esta no canal, se nao estiver ->  ERR_NOTONCHANNEL (442).
// parametros TOPIC <channel> <topic> --> se o parametro topic nao for fornecido retornar o topic do canal RPL_TOPIC e depois RPL_TOPICWHOTIME
// Se <topic> for fornecido altera-se o topico do canal (caso permitido).


