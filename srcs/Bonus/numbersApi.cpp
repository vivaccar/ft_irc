/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numbersApi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:08:27 by aconceic          #+#    #+#             */
/*   Updated: 2025/04/27 19:07:07 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>

static std::string getNumbersApiCategory(std::vector<std::string> &cmds);

//AQUI PRECISO CONSERTAR CASO ALGUEM USE O BOT FORA DO CHANNEL
int	Server::numbersAPI(std::vector<std::string> &cmds, Client *client, Channel *channel)
{
	std::string ret;
	//print_container(cmds, "CMDS CONTAINER");
	std::cout << "NUMBERS API FUNCTION\n";
	std::string category = getNumbersApiCategory(cmds);
	if (category.empty())
	{
		//aqui enviar mensagem de erro na categoria
		//a principio nunca chegara aqui, mas eh bom proteget
		std::cout << "ERRO CATEGORIA\n";
		return (EXIT_FAILURE);
	}
	std::cout << "CATEGORY DEBUG : " << category << std::endl;
	const char *host = "numbersapi.com";
	const char *port = "80";
	std::string path = "/random/" + category;
	
	//1. host
	struct addrinfo hints, *res;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(host, port, &hints, &res) != 0)
	{
		//ENVIAR MENSAGEM DE ERRO NO FETCH
		std::cout << "ERRO NO FETCH " << std::endl;
		return (EXIT_FAILURE);
	}

	//2. criar socket
	int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock < 0)
	{
		freeaddrinfo(res);
		//ENVIAR MENSAGEM DE ERRO NA CRIACAO DO SOCKET
		std::cout << "ERRO NO SOCKET " << std::endl;
		return (EXIT_FAILURE);
	}
	
	if (connect(sock, res->ai_addr, res->ai_addrlen) < 0)
	{
		close(sock);
		freeaddrinfo(res);
		//ENVIAR MENSAGEM DE ERRO NA CONEXAO
		std::cout << "ERRO NA CONEXAO " << std::endl;
		return (EXIT_FAILURE);
	}
	freeaddrinfo(res);
	
	//3. montar e enviar requisicao http
	std::string req = "GET " + path + " HTTP/1.1\r\n"
	"Host: " + host + "\r\n"
	"Connection: close\r\n"
	"\r\n";
	
	send(sock, req.c_str(), req.size(), 0);
	
	//4. receber resposta completa
	std::string resposta;
	char	buffer[1024];
	int		n;

	while ((n = recv(sock, buffer, sizeof(buffer)-1, 0)) > 0)
	{
		buffer[n] = '\0';
		resposta += buffer;	
	}
	close(sock);
	
	//5. separar cabecalhos do corpo
	size_t sep = resposta.find("\r\n\r\n");
	if (sep == std::string::npos)
	{
		//ENVIAR ERRO DE RESPOSTA INVALIDA;
		std::cout << "RESPOSTA INVALIDA \n";
		return (EXIT_FAILURE);
	}
	ret = resposta.substr(sep + 4);
	
	//PRIVMSG #escola :boa tarde
	std::string to_send = ":bot PRIVMSG " + channel->getName() + " :" + ret + "\r\n";
	client->sendToAllChannel(channel, to_send);
	return (EXIT_SUCCESS);
}

//ESSA FUNCAO EH APENAS PARA DEIXAR TUDO EM MAISCULO,
//ACHEI QUE FOSSE NECESSARIA, MAS APARENTEMENTE NAO VAI SER.
static std::string getNumbersApiCategory(std::vector<std::string> &cmds)
{
	std::string ret;
	if (cmds[0] == "trivia" || cmds[0] == "math" || cmds[0] == "year")
	{
		for (std::string::size_type i = 0; i < cmds[0].size(); i++)
		{
			ret += std::tolower(cmds[0][i]);
		}
		return (ret);
	}
	return (ret);
}
