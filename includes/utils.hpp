/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:04:30 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/22 14:18:38 by aconceic         ###   ########.fr       */
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

#endif