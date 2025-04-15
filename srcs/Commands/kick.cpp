#include "../../includes/Server.hpp"
#include "../../includes/tests.hpp"

bool	IsAValidChannel(std::map<std::string, Channel*> channels, std::string name)
{
	std::cout << "IsAValidChannel Mathod\n";
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it ++)
	{
		std::cout << "Name var: " << name << " \n";
		std::cout << "Channel name var: " << it->first << " \n";
		std::cout << "Channel name TO COMPARE: " << name.substr(1) << " \n";

		if (name.substr(1).compare(it->first) == 0)
			return (true);
	}
	return (false);
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
		print_container(cmds, "Size < 3");
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
	//Ainda nao tenho essa funcao
	//check if channel exist
	if (IsAValidChannel(this->_channels, cmds[1]))
	{
		std::cout << "ENTROU NO IF" << std::endl;
	}
		//if yes
				//if yes
					//check if user to be expelled exists and is in channel
						//if yes
							//expell the target user from the channel
							//remove it from channels list
								//if user is the last of the channel
									//delete channel??? maybe.
							//broadcast a messate to the server
						//if no
							//error user not in the channel
		//if no
	//return error msg "no channel"
	return (EXIT_SUCCESS);
}
