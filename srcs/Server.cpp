#include "../includes/Server.hpp"
#include "../includes/utils.hpp"

Server *Server::instance = NULL;

Server::Server(const std::string &port, const std::string &password) {
    this->_port = atoi(port.c_str());
    this->_password = password;
    this->_run = 1;
}

Server::~Server() {
    std::cout << "Server Destroyed!" << std::endl;
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
        delete it->second;
    close(_socketFd);
}

int Server::getPort() const {
    return this->_port;
}

int Server::servRunning() const {
    return this->_run;
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

void Server::signalHandler(int signal) {
    (void)signal;
    std::cout << "ctrl-c received" << std::endl;
    instance->_run = 0;
}

void Server::recSignal() {
    instance = this;
    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, SIG_IGN);
}

Channel *Server::getChannelByName(const std::string &name) {
	std::map<std::string, Channel *>::iterator it = this->_channels.find(name);
	if (it != this->_channels.end())
		return it->second;
	return NULL;
}


void    Server::createSocket() {
    // CRIA O SOCKET DO SERVIDOR
    std::cout << "Creating Server Socket and preparing for receive connections..." <<std::endl;
    _socketFd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET = IPV4, SOCK_STREAM = TCP, 0 = PADRAO
    if (_socketFd <= 0)
        throw("Server Socket Fail");
    _sockAddr.sin_family = AF_INET;
    _sockAddr.sin_port = htons(_port);
    _sockAddr.sin_addr.s_addr = INADDR_ANY;

    int en = 1;
    if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //HABILITAR A REUTILIZACAO DA PORTA ASSIM QUE FOR FECHADO
        throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    
    if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    
    // FAZ O SOCKET ESCUTAR NA PORTA E IP DESEJADOS
    if (bind(_socketFd, (struct sockaddr *)&_sockAddr, sizeof(_sockAddr)) < 0)
        throw (std::runtime_error("Bind Fail"));
    
    // COLOCA O SOCKET DO SERVIDOR EM MODO ESCUTA
    if (listen(_socketFd, __INT_MAX__) < 0)
        throw(std::runtime_error("Listen Fail"));
}

void    Server::createClient(int socket) {
    Client *newClient = new Client(socket);
    this->_clients.insert(std::make_pair(socket, newClient));
}

void	Server::privMsg(std::vector<std::string> &cmds, Client *client) {
	//verificar se eh para canal ou user
	if (cmds[1][0] == '#') {
		Channel *channel = getChannelByName(cmds[1]); // verificar se o nome do canal esta sendo armazenado com '#'
		if (channel && client->isChannelMember(channel)) {
			if (channel != NULL && cmds[2][0] == ':') {
                std::string msg = ":" + client->getNick() + " PRIVMSG " + channel->getName() + " ";
				for (size_t i = 2; i < cmds.size(); i++)
					msg+= cmds[i] + " ";
				msg += "\n";
				client->sendToChannel(channel, msg);
			}
		}
	}	
	else {
		Client *dest = this->getClientByNick(cmds[1]);
		if (dest != NULL && cmds[2][0] == ':') { //O USER DESTINO EXISTE
            std::string msg = ":" + client->getNick() + " PRIVMSG " + dest->getNick() + " ";
            //:Angel PRIVMSG Wiz :Hello are you receiving this message ?
			//std::string msg = "@" + client->getNick() + " sent a private message to you";
			for (size_t i = 2; i < cmds.size(); i++)
				msg+= cmds[i] + " ";
			msg+= "\n";
			client->sendToClient(dest, msg);
		}
		else if (dest == NULL) {
			std::string msg = "User @" + cmds[1] + " not found!\n";
			client->sendToClient(client, msg);
		}
		else {
			std::string msg = "Invalid Synthax\nThe command must be \"PRIVMSG <nickname> : <message>\"\n";
			client->sendToClient(client, msg);
		}
	}

}

void    Server::parseCommand(std::string cmd, int clientSocket) {
    
    std::string message;
    size_t i = cmd.find(':');
    if (i != std::string::npos)
        message = cmd.substr(i, cmd.size());
    std::cout << BLUE << message << RESET << std::endl;
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
		
		std::cout << cmds[0] << std::endl;
        if (cmds[0] == "PASS" || cmds[0] == "pass")
            checkPassword(cmds, client);
        else if (cmds[0] == "NICK" || cmds[0] == "nick")
            setNick(cmds, client);
        else if (cmds[0] == "USER" || cmds[0] == "user")
            setUser(cmds, client);
        else if (!client->isAuth())
            sendResponse(client->getSocket(), ERR_NOTREGISTERED(client->getNick()));
        else if (cmds[0] == "TOPIC" || cmds[0] == "topic")
                topic(cmds, client, cmd);
        else if (cmds[0] == "MODE" || cmds[0] == "mode")
                mode(cmds, client, cmd);
        else if (cmds[0] == "JOIN")
        {
            joinCommand(cmds, client);
            /* std::string joinresp = ":" + client->getNick() + " JOIN " + cmds[1] + "\n";
            send(clientSocket, joinresp.c_str(), strlen(joinresp.c_str()), 0); */
        }
        else if (cmds[0] == "PRIVMSG")
            privMsg(cmds, client);
		else if (cmds[0] == "KICK") //TO START WORKING AT THE COMMANDS REQUIRED BY THE SUBJECT
			kickUser(cmds, client);
		else if (cmds[0] == "INVITE")
			inviteUser(cmds, client);
		else if (cmds[0] == "trivia" || cmds[0] == "math" || cmds[0] == "year")
		{
			if (cmds.size() < 2 || cmds[1].empty()) //precisa necessariamente ter o nome do canal
				continue ;

			Channel *channel = ReturnChannel(this->_channels, cmds[1], client);
			if (!channel)
				continue;
			numbersAPI(cmds, client, channel);
			//client->sendToClient(client, to_send);
			//client->sendToAllChannel();
		}
        else
            sendResponse(client->getSocket(), ERR_UNKNOWNCOMMAND(client->getNick(), cmds[0]));
    }
}

void    Server::runPoll() {
    // CRIA A ESTRUTURA DO DO SOCKET DO SERVIDOR PARA SER UTILIZADO NO POLL
    struct pollfd server;
    server.fd = _socketFd;
    server.events = POLLIN;
    server.revents = 0;
    _fds.push_back(server);
    
    std::cout << std::endl << GREEN << "Server waiting connections on FD "
        << _socketFd << RESET << std::endl;
    while (_run)
    {
        int ret = poll(_fds.data(), _fds.size(), 0);
        if (ret < 0 && _run)
            throw(std::runtime_error("Poll Error"));
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
    std::cout << RED << "SERVER IS DOWN - free memory here" << RESET << std::endl;
}

void	Server::sendResponse(int socket, const std::string &response) const {
    send(socket, response.c_str(), strlen(response.c_str()), 0);
}
