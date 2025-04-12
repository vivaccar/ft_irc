#include "../includes/Server.hpp"

Server::Server(const std::string &port, const std::string &password) {
    this->_port = atoi(port.c_str());
    this->_password = password;
}

Server::~Server() {
    std::cout << "Server Destroyed!" << std::endl;
}

int Server::getPort() const {
    return this->_port;
}

const std::string& Server::getPassword() const {
    return this->_password;
}

std::map<int, Client*> Server::getClientsMap() const{
	return this->_clients;
}

Client* Server::getClientBySocket(int socket) {
    std::map<int, Client*>::iterator it = _clients.lower_bound(socket);
    if (it != _clients.end())
        return it->second;
    return NULL;
}

Client* Server::getClientByNick(const std::string &nick) {
	std::map<int, Client*> c = this->getClientsMap();
	for (std::map<int, Client *>::iterator it = c.begin(); it != c.end(); it++) {
		if (it->second->getNick() == nick)
			return it->second;
	}
	return NULL;
} 

void    Server::createSocket() {
    // CRIA O SOCKET DO SERVIDOR
    std::cout << "Creating socket" <<std::endl;
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd <= 0)
        throw("Server Socket Fail");
    _sockAddr.sin_family = AF_INET;
    _sockAddr.sin_port = htons(_port);
    _sockAddr.sin_addr.s_addr = INADDR_ANY;

    // FAZ O SOCKET ESCUTAR NA PORTA E IP DESEJADOS
    if (bind(_socketFd, (struct sockaddr *)&_sockAddr, sizeof(_sockAddr)) < 0)
        throw ("Bind Fail");
    
    // COLOCA O SOCKET DO SERVIDOR EM MODO ESCUTA
    if (listen(_socketFd, __INT_MAX__) < 0)
        throw("Listen Fail");
}

void    Server::createClient(int socket) {
    Client *newClient = new Client(socket);
    this->_clients.insert(std::make_pair(socket, newClient));
}


void    Server::checkPassword(std::vector<std::string> &cmds, Client *client)
{
    if (cmds.size() == 2)
    {
        if (cmds[1] == _password)
        {
            std::cout << "Client " << client->getSocket() << " type the correct password!" << std::endl;
            client->setInsertPassword(true);
			const char *response = "Correct password! You are connected to the server\nNow insert your nickname\n";
            send(client->getSocket(), response, strlen(response), 0);
        }
        else
            std::cout << "Client " << client->getSocket() << " type the incorrect password!" << std::endl;
			const char *response = "Incorrect password!\nTry again\n";
            send(client->getSocket(), response, strlen(response), 0);
    }
}

void    Server::setNick(std::vector<std::string> &cmds, Client *client)
{
	if (cmds.size() > 1) {
		client->setNick(cmds[1]);
		std::cout << "Client " << client->getSocket() << " :" << " set new NICKNAME :" << client->getNick() << std::endl;
		std::string response = "Nickname set!\nWelcome " + client->getNick() + " to the IRC server!\n";
		send(client->getSocket(), response.c_str(), response.size(), 0);
	}
}

void    Server::setUser(std::vector<std::string> &cmds, Client *client)
{
	client->setUser(cmds[1]);
	std::cout << "Client " << client->getSocket() << " :" << " set new USERNAME :" << client->getUser() << std::endl;
	const char* response = "Username set!\nNow you are able to join/create a channel\n";
	send(client->getSocket(), response, strlen(response), 0);
}

void    Server::joinCommand(std::vector<std::string> &cmds, Client *client) {
	std::map<std::string, Channel *>::iterator it = this->_channels.find(cmds[1]);
	if (it == _channels.end()) {
		Channel *newChannel = client->createChannel(cmds[1]);
		this->_channels.insert(std::make_pair(newChannel->getName(), newChannel));
	}
	else {
		int ret = client->joinChannel(it->second);
		//mensagens de erro serao tratadas aqui, dependendo do retorno de joinChannel
		(void) ret;


	}
	
}

void	Server::privMsg(std::vector<std::string> &cmds, Client *client) {
	//verificar se eh para canal ou user
	/* if (cmds[1][0] == '#')
		msgChannel */
	Client *dest = this->getClientByNick(cmds[1]);
	if (dest != NULL) { //O USER DESTINO EXISTE
		std::string msg = "@" + client->getNick() + " sent a private message to you: ";
		for (size_t i = 3; i < cmds.size(); i++)
			msg+= cmds[i] + " ";
		msg+= "\n";
		send(dest->getSocket(), msg.c_str(), msg.size(), 0);
	}

}

void    Server::parseCommand(std::string cmd, int clientSocket) {
    std::vector<std::string> cmds;
    std::istringstream stream(cmd);
    std::string word;
    
    while (stream >> word)
        cmds.push_back(word);

    Client *client = getClientBySocket(clientSocket);
	if (cmds.size() > 1) {
		if (cmds[0] == "PASS")
			checkPassword(cmds, client);
		else if (cmds[0] == "NICK")
			setNick(cmds, client);
		else if (cmds[0] == "USER")
			setUser(cmds, client);
		else if (cmds[0] == "JOIN")
			joinCommand(cmds, client);
		else if (cmds[0] == "PRIVMSG")
			privMsg(cmds, client);
	}
}

void    Server::runPoll() {
    // CRIA A ESTRUTURADO DO SOCKET DO SERVIDOR PARA SER UTILIZADO NO POLL
    struct pollfd server;
    server.fd = _socketFd;
    server.events = POLLIN;
    server.revents = 0;
    _fds.push_back(server);
    
    while (1)
    {
        int ret = poll(_fds.data(), _fds.size(), 0);
        if (ret < 0)
            throw("Poll Error");
        if (_fds[0].revents & POLLIN)
        {
            
            int newClientSock = accept (_socketFd, NULL, NULL);
            if (newClientSock < 0)
            {
                std::cout << "Fail to accept connection" << std::endl;
                continue;
            }
            createClient(newClientSock);
            struct pollfd newClient;
            newClient.fd = newClientSock;
            newClient.events = POLLIN;
            newClient.revents = 0;
            _fds.push_back(newClient);
            const char *login = "To authenticate, follow the steps\n1 - PASS [password]\n2 - NICK [nickname]\n3 - USER [username]\n";
            send(newClientSock, login, strlen(login), 0);
            std::cout << "New client connected on FD: " << newClient.fd << std::endl;
        }
        for (size_t i = 1; i < _fds.size(); i++)
        {  // Começa no índice 1, já que o índice 0 é o servidor
            if (_fds[i].revents & POLLIN)
            {
                char buffer[1024] = {0};
                int client_socket = _fds[i].fd;
                int bytes_read = recv(client_socket, buffer, 1024, 0);
                if (bytes_read == 0)
                {
                    // Cliente desconectou
                    std::cout << "Client desconected on FD" << _fds[i].fd << std::endl;
                    close(client_socket);
                    _fds.erase(_fds.begin() + i);  // Remove o cliente da lista
                    --i;  // Ajusta o índice após a remoção
                } else
                {
                    // Envia resposta ao cliente
                    std::cout << "Client " << client_socket << " say:" << buffer << std::endl;
                    parseCommand(std::string(buffer), client_socket);
                    const char* response = "Message received by the server!\n";
                    send(client_socket, response, strlen(response), 0);
                }
            }
        }
    }
}
