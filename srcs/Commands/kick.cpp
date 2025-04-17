#include "../../includes/Server.hpp"
#include "../../includes/tests.hpp"


//Aqui a intencao eh buscar dentro da lista de todos os channels do server
//e verificar se o server existe
//tendo em consideracao o nome do servidor a ser procurado
//(que eh o comando dado no KICK)
static bool	IsAValidChannel(std::map<std::string, Channel*> channels, std::string name)
{
	std::cout << "IsAValidChannel Method\n";
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it ++)
	{
		if (name.substr(1).compare(it->first) == 0)
			return (true);
	}
	return (false);
}

//Aqui a intencao eh buscar o channel por nome para verificar se o client que esta
//tentando realizar a acao eh um admin ou nao
static bool	isClientOperator(Client *client, std::map<std::string, Channel*> channels, std::string channel_name)
{
	std::map<std::string, Channel*>::iterator channel_found = channels.begin(); //iguala iterador ao comeco
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it ++) //roda por todos os canais
	{
		if (channel_name.substr(1).compare(it->first) == 0) //se encontrar o canal
		{
			channel_found = it; // o iterador indicando que achou o canal aponta para ele
			break;
		}
	}

	if (channel_found == channels.end()) // se rodar todos os canais e nao achar nada, canal invalido.
		return (false);

	std::vector<int> admins = channel_found->second->getAdmins(); //busca os admins do canal encontrado
	if (std::find(admins.begin(), admins.end(), client->getSocket()) != admins.end()) //se achar o admin, retorna true
		return (true);

	return (false);
}

//Aqui a intencao eh procurar o user escrito pelo Operator e verificar se ele existe e esta no canal

bool	isTargetUserOnChannel(std::map<std::string, Channel*> channels, std::string channel_name, std::string Target)
{
	(void)channels;
	(void)channel_name;
	(void)Target;
	return (true);
}

int	Server::kickUser(std::vector<std::string> &cmds, Client *client)
{
	std::cout << "kickUser method \n";

	(void)client;
	//command validation
	//if command is invalid (DOES NOT FOLLOW THIS -> KICK #channel targetUser [:reason])
		//return (does not have correct arguments)
	if (cmds.size() < 3)
	{
		//print_container(cmds, "Size < 3");
		//Broadcast here msg de kick invalido para todos os usuarios
		std::cout << "Not a valid KICK command, try KICK #channel targetUser [:reason]" << std::endl;
		return (EXIT_FAILURE);
	}
	//[:reason] is optional. Define a default generic reason in case where the user didnt define anything.
	else if (cmds.size() == 3)
	{
		cmds.push_back("Default reason\n");
		print_container(cmds, "cmds == 3 \n");
	}

	// AQUI verificar se quem esta querendo kickar eh operator
	//check if channel exist
	//cmds[1] em tese eh o nome do canal (seguindo isso KICK #channel targetUser [:reason])
	if (IsAValidChannel(this->_channels, cmds[1]) && isClientOperator(client, this->_channels, cmds[1]))
	{
		std::cout << "ENTROU NO IF DE CANAL VALIDO, CLIENTE OPERATOR \n";
		//if yes
			//check if user to be expelled exists and is in channel
				//if yes
					//expell the target user from the channel
					//remove it from channels list <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
						//send(seguindo aquela strin do rawlog)
						//if user is the last of the channel
							//delete channel??? maybe.
					//broadcast a messate to the server
				//if no
					//error user not in the channel
	}
	else
	{
		//if no
		//return error msg "no channel"
		std::cout << "NAO ENTROU NO IF DE CANAL VALIDO, CLIENTE OPERATOR\n";
	}
		
		
	return (EXIT_SUCCESS);
}
