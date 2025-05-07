/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:04:30 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/25 13:53:19 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include "../includes/Server.hpp"

/**************************************/
/*              TEMPLATES             */
/**************************************/
template <typename T>
void print_container(T cont, std::string msg)
{
	if (!msg.empty())
	{
		std::cout << msg << std::endl;
	}
	for (typename T::iterator it = cont.begin(); it != cont.end(); it ++)
	{
		std::cout << *it << std::endl;
	}
}

/**************************************/
/*              DECLARATIONS          */
/**************************************/

//std::string extractMessage(std::string cmd, int maxSpaces); this is declared on Served.hpp

std::vector<std::string> 	splitVectorString(const std::vector<std::string>& strs);
bool    					onlyNumbers(const std::string &str);


#endif