/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numbersApi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aconceic <aconceic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:08:27 by aconceic          #+#    #+#             */
/*   Updated: 2025/05/03 18:21:18 by aconceic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"
#include <cstring> //std::memset
#include <netdb.h>
#include <cerrno> 

static std::string getNumbersApiCategory(std::vector<std::string> &cmds);

static int createAndConnectSocketBOT(addrinfo *res);
static int prepareHost(addrinfo &hints, addrinfo *&res, const char* host, const char* port);
static int doAndSendHttpRequest(int socket, std::string path, const char *host);
static std::string receiveResponseFromAPI(int socket);
static int sendToClientError (Client *client, std::string &err_msg);

//AQUI PRECISO CONSERTAR CASO ALGUEM USE O BOT FORA DO CHANNEL
int	Server::numbersAPI(std::vector<std::string> &cmds, Client *client, Channel *channel)
{
	std::string APIresponse;
	std::string err_msg = ": NOTICE " + client->getNick() 
							+ " :Can't connect with external source.\r\n";
	std::string category = getNumbersApiCategory(cmds);

	if (category.empty())
		return (sendToClientError(client, err_msg));
	
	const char *host = "numbersapi.com";
	const char *port = "80"; //80 - por ta padrao de HTTP nao criptografado
	std::string path = "/random/" + category;
	
	//1. host
	struct addrinfo hints, *res = NULL;
	if (prepareHost(hints, res, host, port))
		return (sendToClientError(client, err_msg));

	//2. create socket
	int sock = createAndConnectSocketBOT(res);
	if (sock == -1)
		return (sendToClientError(client, err_msg));

	//3. create & send http request
	if (doAndSendHttpRequest(sock, path, host))
		return (sendToClientError(client, err_msg));
	
	//4. Receive response from API and convert to string
	APIresponse = receiveResponseFromAPI(sock);
	if (APIresponse.size() < 1)
		return (sendToClientError(client, err_msg));

	//PRIVMSG #escola :boa tarde
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
	hints.ai_socktype = SOCK_STREAM; //SOCKETS PARA TCP

	if (getaddrinfo(host, port, &hints, &res) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//second step is to create the BOT socket
static int createAndConnectSocketBOT(addrinfo *res)
{
	int sock = socket(res->ai_family, res->ai_socktype, 0);
	if (sock < 0)
	{
		freeaddrinfo(res);
		return (-1);
	}
	
	//AQUI PRECISO TORNAR NON BLOCKING
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) //make non blocking
	{
		std::cout << "PAROU AQUI \n";
		close(sock);
		return (-1);
	}

	int status = connect(sock, res->ai_addr, res->ai_addrlen);
	if (status < 0 && errno != EINPROGRESS)
	{
		close(sock);
		freeaddrinfo(res);
		return (-1);
	}

	struct pollfd pfd;
	pfd.fd = sock;
	pfd.events = POLLOUT;
	pfd.revents = 0;
	int	ret;

	while (true)
	{
		ret = poll(&pfd, 1, 5000);
		if (ret < 0)
		{
			if (errno == EINTR)
				continue;
			close(sock);
			freeaddrinfo(res);
			return (-1);
		}
		if (ret == 0)
		{
			std::cerr << "Timeout no connect()\n";
            close(sock);
            freeaddrinfo(res);
            return -1;
		}
		break ;
	}
	
	int err = 0;
    socklen_t len = sizeof(err);
    if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
        close(sock);
        freeaddrinfo(res);
        return -1;
    }
	
    if (err != 0) {
        std::cerr << "Erro no connect(): " << std::strerror(err) << "\n";
        close(sock);
        freeaddrinfo(res);
        return -1;
    }

    // Conexão estabelecida com sucesso!
	freeaddrinfo(res);
	std::cout << "CONEXAO FEITA!\n";
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

	while (true)
	{
		int n = recv(socket, buffer, sizeof(buffer)-1, 0);
		if (n == 0)
			break;
		if (n == -1)
		{
			if (errno == EWOULDBLOCK)
				continue;
			else
				break;	
		}
		std::cout << "TESTE \n";
		buffer[n] = '\0';
		resposta += buffer;	
	}
	close(socket);

	std::cout << "RESPOSTA : " << resposta << "\n";
	//5. separar cabecalhos do corpo
	size_t sep = resposta.find("\r\n\r\n");
	if (sep == std::string::npos)
		return ("");

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