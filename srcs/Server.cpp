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
    std::cout << "Creating Server Socket and preparing for receive connections..." <<std::endl;
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
    std::vector<std::string> lines;
    std::istringstream streamLine(cmd);
    std::string line;
    
    while (std::getline(streamLine, line))
        lines.push_back(line);
    Client *client = getClientBySocket(clientSocket);
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) 
    {
        std::vector<std::string> cmds;
        std::string word;
        std::istringstream streamCmd(*it);
        while (streamCmd >> word)
            cmds.push_back(word);
        if (cmds[0] == "PASS" || cmds[0] == "pass")
            checkPassword(cmds, client);
        else if (cmds[0] == "NICK" || cmds[0] == "nick")
            setNick(cmds, client);
        else if (cmds[0] == "USER" || cmds[0] == "user")
            setUser(cmds, client);
        else if (!client->isAuth())
            sendResponse(client->getSocket(), ERR_NOTREGISTERED(client->getNick()));
        else if (cmds[0] == "JOIN")
        {
            joinCommand(cmds, client);
            std::string joinresp = ":" + client->getNick() + " JOIN " + cmds[1] + "\n";
            send(clientSocket, joinresp.c_str(), strlen(joinresp.c_str()), 0);
        }
        else if (cmds[0] == "PRIVMSG")
            privMsg(cmds, client);
        else
            sendResponse(client->getSocket(), ERR_UNKNOWNCOMMAND(client->getNick(), cmds[0]));
    }
}

void    Server::runPoll() {
    // CRIA A ESTRUTURADO DO SOCKET DO SERVIDOR PARA SER UTILIZADO NO POLL
    struct pollfd server;
    server.fd = _socketFd;
    server.events = POLLIN;
    server.revents = 0;
    _fds.push_back(server);
    
    std::cout << std::endl << GREEN << "Server waiting connections on FD "
        << _socketFd << RESET << std::endl;
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
            std::cout << GREEN << std::endl << "New client connected on FD: "
                << newClient.fd << RESET << std::endl;
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
                    Client *toDelete = getClientBySocket(client_socket);
                    if (toDelete)
                        delete toDelete;
                    std::cout << RED << std::endl << "Client desconected on FD" << _fds[i].fd << RESET << std::endl;
                    close(client_socket);
                    _fds.erase(_fds.begin() + i);  // Remove o cliente da lista
                    _clients.erase(client_socket);
                    --i;  // Ajusta o índice após a remoção
                } else
                {
                    // Envia resposta ao cliente
                    std::cout << "\nClient " << client_socket << " say: " << buffer << std::endl;
                    parseCommand(std::string(buffer), client_socket);
                }
            }
        }
    }
}

void	Server::sendResponse(int socket, const std::string &response) const {
    send(socket, response.c_str(), strlen(response.c_str()), 0);
}
