/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfassbin <mfassbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:12:43 by aconceic          #+#    #+#             */
/*   Updated: 2025/05/05 13:00:10 by mfassbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/utils.hpp"

bool    onlyNumbers(const std::string &str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

//This one is declared on Server.hpp
std::string extractMessage(std::string cmd, int maxSpaces)
{
    int index = 0;
    int spaces = 0;

    while (std::isspace(cmd[index]))
        index++;
    while (spaces < maxSpaces) 
    {
        if (std::isspace(cmd[index]))
        {
            while (std::isspace(cmd[index]))
                index++;
            spaces++;
        }
        else 
            index++;
    }
    if (cmd[index] == ':')
        return cmd.substr(index + 1, cmd.size());
    return cmd.substr(index, cmd.size());
}


/**********************************************/
/*              AMAURI DECLARATIONS           */
/**********************************************/
//used on kick.cpp
//Split a vector of strings using a "," as separator.
//Return a vector with all the strings.
std::vector<std::string> splitVectorString(const std::vector<std::string>& strs)
{
    std::vector<std::string> client_users;

    for (std::vector<std::string>::const_iterator it = strs.begin(); it != strs.end(); ++it)
    {
        const std::string& s = *it;
        std::string::size_type start = 0;

        while (true)
        {
            std::string::size_type end = s.find(',', start);
            client_users.push_back(s.substr(start, end - start));

            if (end == std::string::npos)
                break;

            start = end + 1;
        }
    }
    return (client_users);
}


std::vector<std::string> splitString(std::string &str) {
	size_t start = 0;
	size_t end = 0;
	std::vector<std::string> vec;
	while(end != str.size()) {
		end = str.find(",", start);
		if (end == str.npos)
			end = str.size();
		vec.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	return vec;
}