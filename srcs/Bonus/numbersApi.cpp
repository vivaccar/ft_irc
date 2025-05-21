/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numbersApi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:08:27 by aconceic          #+#    #+#             */
/*   Updated: 2025/05/21 18:38:56 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"
#include <cstring> //std::memset
#include <netdb.h>

static std::string getNumbersApiCategory(std::vector<std::string> &cmds);

static int createSocketBOT(addrinfo *res);
static int prepareHost(addrinfo &hints, addrinfo *&res, const char* host, const char* port);
static int doAndSendHttpRequest(int socket, std::string path, const char *host);
static std::string receiveResponseFromAPI(int socket);
static int sendToClientError (Client *client, std::string &err_msg);

int	Server::numbersAPI(std::vector<std::string> &cmds, Client *client, Channel *channel)
{

	std::string APIresponse;
	std::string err_msg = ": NOTICE " + client->getNick() 
							+ " :Can't connect with external source.\r\n";
	std::string category = getNumbersApiCategory(cmds);

	if (category.empty())
		return (sendToClientError(client, err_msg));
	
	const char *host = "numbersapi.com";
	const char *port = "80"; //80 - standard port of HTTP non criptoooap[osa]
	std::string path = "/random/" + category;
	
	//1. host
	struct addrinfo hints, *res = NULL;
	if (prepareHost(hints, res, host, port))
		return (sendToClientError(client, err_msg));

	//2. create socket
	int sock = createSocketBOT(res);
	if (sock == -1)
		return (sendToClientError(client, err_msg));

	//3. create & send http request
	if (doAndSendHttpRequest(sock, path, host))
		return (sendToClientError(client, err_msg));
	
	//4. Receive response from API and convert to string
	APIresponse = receiveResponseFromAPI(sock);
	if (APIresponse.empty())
		return (sendToClientError(client, err_msg));

	//PRIVMSG #escola :good afternoon
	std::string to_send = ":bot PRIVMSG " + channel->getName() + " :" + APIresponse + "\r\n";
	client->sendToAllChannel(channel, to_send);
	return (EXIT_SUCCESS);
}

//The only work of this function is to make everything lowercase
//I tought this was going to be necessary. But is not (at least for now).
//I am going to leave it here (for now)
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

//First step is to prepare de host
static int prepareHost(addrinfo &hints, addrinfo *&res, const char* host, const char* port)
{
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; //IPV4
	hints.ai_socktype = SOCK_STREAM; //SOCKETS FOR TCP

	if (getaddrinfo(host, port, &hints, &res) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//second step is to create the BOT socket
static int createSocketBOT(addrinfo *res)
{
	int sock = socket(res->ai_family, res->ai_socktype, 0);
	if (sock < 0)
	{
		freeaddrinfo(res);
		return (-1);
	}
	
	if (connect(sock, res->ai_addr, res->ai_addrlen) < 0)
	{
		close(sock);
		freeaddrinfo(res);
		return (-1);
	}
	freeaddrinfo(res);
	return (sock);
}

//Third Step is to create and send HTTP request
static int doAndSendHttpRequest(int socket, std::string path, const char *host)
{
	std::string request = "GET " + path + " HTTP/1.1\r\n"
	"Host: " + host + "\r\n"
	"Connection: close\r\n"
	"\r\n";
	
	ssize_t len = send(socket, request.c_str(), request.size(), 0);
	if (len < 0)
		return(EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//fourth step is to receive the response and create the return string
static std::string receiveResponseFromAPI(int socket)
{
	std::string resposta;
	std::string ret;
	char	buffer[1024];
	int		n;

	while ((n = recv(socket, buffer, sizeof(buffer)-1, 0)) > 0)
	{
		buffer[n] = '\0';
		resposta += buffer;	
	}
	close(socket);

	//5. Separate cabecalhos from body
	size_t sep = resposta.find("\r\n\r\n");
	if (sep == std::string::npos)
		return (NULL);

	ret = resposta.substr(sep + 4);
	return (ret);
};

//Wrapper function to make the code concise without modifying code from other collegues
//I've made it just because i needed this function to return value and the 
//original sendToClient is void.
static int sendToClientError(Client *client, std::string &err_msg)
{
	client->sendToClient(client, err_msg.c_str());
	return (EXIT_FAILURE);
}