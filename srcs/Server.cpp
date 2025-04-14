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

Client* Server::getClient(int socket) {
    std::map<int, Client*>::iterator it = _clients.lower_bound(socket);
    if (it != _clients.end())
        return it->second;
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
        }
        else
            std::cout << "Client " << client->getSocket() << " type the incorrect password!" << std::endl;
    }
}

void    Server::setNick(std::vector<std::string> &cmds, Client *client)
{
        client->setNick(cmds[1]);
        std::cout << client->getSocket() << " :" << " set new NICKNAME :" << client->getNick() << std::endl;
}

void    Server::setUser(std::vector<std::string> &cmds, Client *client)
{
        client->setUser(cmds[1]);
        std::cout << client->getSocket() << " :" << " set new USERNAME :" << client->getUser() << std::endl;
}

void    Server::parseCommand(std::string cmd, int clientSocket) {
    std::vector<std::string> cmds;
    std::istringstream stream(cmd);
    std::string word;
    

    (void) clientSocket;
    while (stream >> word)
        cmds.push_back(word);

    Client *client = getClient(clientSocket);
    if (cmds[0] == "PASS")
        checkPassword(cmds, client);
    else if (cmds[0] == "NICK")
        setNick(cmds, client);
    else if (cmds[0] == "USER")
        setUser(cmds, client);
	else if (cmds[0] == "KICK") //TO START WORKING AT THE COMMANDS REQUIRED BY THE SUBJECT
		kickUser(cmds, client);
	/*
	// (PASS, NICK, USER, JOIN, PART, TOPIC, INVITE, KICK, QUIT, MODE, and PRIVMSG)
	else if (cmds[0] == "KICK") //TO START WORKING AT THE COMMANDS REQUIRED BY THE SUBJECT
		//kick function
		kickUser(cmds, client);
	else if (cmds[0] == "INVITE")
		//invite function
	else if (cmds[0] == "TOPIC") 
		//topic function
	else if (cmds[0] == "MODE")
		//mode function, where
			//i: Set/Remove Invite-only channel
			//t: set/remove the restrictions of the TOPIC command to channel operator
			//k: set/remove the channel key (password)
			//o: give/take channel operator(moderador) privilege
			//l: set/remove the user limit to channel
	*/
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
                    std::cout << "Client " << _fds[i].fd << " say:" << buffer << std::endl;
                    parseCommand(std::string(buffer), client_socket);
                    const char* response = "Message received by the server!";
                    send(client_socket, response, strlen(response), 0);
                }
            }
        }
    }
}
