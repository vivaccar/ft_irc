#include "../includes/Server.hpp"

void	Server::kickUser(std::vector<std::string> &cmds, Client *client)
{

	//command validation
	//if command is invalid (DOES NOT FOLLOW THIS -> KICK #channel targetUser [:reason])
		//return (does not have correct arguments)
	//[:reason] is optional. Define a default generic reason in case where the user didnt define anything.
	
	//check if channel exist
		//if yes
			//check if user who gave the command is an operator
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
					//return error msg user not operator
		//if no
	//return error msg "no channel"
}