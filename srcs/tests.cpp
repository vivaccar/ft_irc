/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:01:51 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/14 15:50:26 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tests.hpp"

#define ERROR 1
#define SUCCESS 0

//This function is being ignored for now
int	kickUserTest(Server s)
{
	(void)s;
	std::vector<std::string> cmds;
	cmds.push_back("cmd0");
	cmds.push_back("cmd1");
	cmds.push_back("cmd2");
	cmds.push_back("cmd3");
	
	Client *client = NULL;

	s.kickUser(cmds, client);
	return (SUCCESS);
}