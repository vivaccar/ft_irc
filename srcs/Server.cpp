#include "../includes/Server.hpp"
#include "../includes/utils.hpp"

Server *Server::instance = NULL;

Server::Server(const std::string &port, const std::string &password) {
    this->_port = atoi(port.c_str());
    this->_password = password;
    this->_run = 1;
}

Server::~Server() {
    //log("Server Destroyed");
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
        delete it->second;
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
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

std::map<int, Client*> &Server::getClientsMap(){
	return this->_clients;
}

std::map<std::string, Channel *> &Server::getChannelsMap(){
	return this->_channels;
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
    instance->_run = 0;
}

void Server::recSignal() {
    instance = this;
    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}

Channel *Server::getChannelByName(const std::string &name) {
	std::map<std::string, Channel *>::iterator it = this->_channels.find(name);
	if (it != this->_channels.end())
		return it->second;
	return NULL;
}


void    Server::createSocket() {
    // CRIA O SOCKET DO SERVIDOR
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

void    Server::createClient(int socket, std::string &hostname) {
    Client *newClient = new Client(socket, hostname);
    this->_clients.insert(std::make_pair(socket, newClient));
}

void    Server::parseCommand(std::string cmd, int clientSocket, size_t &pollIdx) {
    std::istringstream streamLine(cmd);

    Client *client = getClientBySocket(clientSocket);
    std::vector<std::string> cmds;
    std::string word;
    std::istringstream streamCmd(cmd);
    while (streamCmd >> word)
        cmds.push_back(word);
    if (cmds.size() == 0)
        return ;
    if (cmds[0] == "QUIT" || cmds[0] == "quit")
        disconnectClient(clientSocket, pollIdx);
    else if (cmds[0] == "PASS" || cmds[0] == "pass")
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
    else if (cmds[0] == "JOIN" || cmds[0] == "join")
        joinCommand(cmds, client);
    else if (cmds[0] == "PRIVMSG" || cmds[0] == "privmsg")
        privMsg(cmds, client, cmd);
	else if (cmds[0] == "WHO" || cmds[0] == "who")
        who(cmds, client);
    else if (cmds[0] == "KICK" || cmds[0] == "kick") //TO START WORKING AT THE COMMANDS REQUIRED BY THE SUBJECT
        kickUser(cmds, client);
    else if (cmds[0] == "INVITE" || cmds[0] == "invite")
        inviteUser(cmds, client);
	else if (cmds[0] == "trivia" || cmds[0] == "math" || cmds[0] == "date" || cmds[0] == "year")
	{
		if (cmds.size() < 2 || cmds[1].empty()) //precisa necessariamente ter o nome do canal
			return ;
		Channel *channel = getChannelByName(cmds[1]);
		if (!channel)
			return ;
		numbersAPI(cmds, client, channel);
	}
    else
        sendResponse(client->getSocket(), ERR_UNKNOWNCOMMAND(client->getNick(), cmds[0]));
    
}

void    Server::connectNewClient() {
    struct sockaddr_in clientAddr;
    socklen_t           clientAddrLen = sizeof(clientAddr);
    
    int newClientSocket = accept(_socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newClientSocket < 0)
        return log("Fail to accept connection");
    if (fcntl(newClientSocket, F_SETFL, O_NONBLOCK) == -1)
        return log("Fail to set client FD Non Blocking");
    std::string hostname = inet_ntoa(clientAddr.sin_addr);
    createClient(newClientSocket, hostname);
    struct pollfd newClient;
    newClient.fd = newClientSocket;
    newClient.events = POLLIN;
    newClient.revents = 0;
    _fds.push_back(newClient);
    
    std::stringstream message;
    message << "New client succesfully connected on Socket: " << newClientSocket;
    log(message.str());
}

void	Server::deleteChannelWithNoClients() {
	std::map<std::string, Channel *> &channelMap = getChannelsMap();
	std::map<std::string, Channel *>::iterator it = channelMap.begin();
	while (it != channelMap.end())
	{
		Channel *channel = it->second;
		if(channel->getClients().size() == 0) {
			std::map<std::string, Channel *>::iterator itToErase = it;
			it++;
			channelMap.erase(itToErase);
			delete channel;
		}
		else
			it++;
	}
}

void    Server::disconnectClient(int fd, size_t &poolIdx)
{
    Client* toDelete = getClientBySocket(fd);
    if (toDelete)
        delete toDelete;
    deleteChannelWithNoClients();
    _clients.erase(fd);
    close(fd);
    _fds.erase(_fds.begin() + poolIdx);  // Remove o cliente da lista
    _clients.erase(poolIdx);
    --poolIdx;
    std::stringstream message;
    message << "Client disconnected Socket: " << fd;
    log(message.str());
}

void Server::readNewMessage(size_t &pollIdx)
{
    char buffer[1024];
    bzero(buffer, sizeof(buffer));
    int fd = _fds[pollIdx].fd;

    std::string msg;
    ssize_t bytesRead;
    while (true)
    {
        bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead == -1)
        {
            if (errno == EWOULDBLOCK)
                break;
            else
                throw std::runtime_error("Recv Error");
        }
        else if (bytesRead == 0)
            return disconnectClient(fd, pollIdx);
        buffer[bytesRead] = '\0';
        msg.append(buffer);
        if (msg.find('\n') != std::string::npos)
            break;
    }
    std::istringstream iss(msg);
    std::string line;
    while (std::getline(iss, line)) 
    {
        // remove \r se vier no final (de \r\n)
        if (line.empty() || (line[0] == '\r' && line.size() == 1))
            continue;
        if (!line.empty() && line[line.size() -1] == '\r')
            line = line.substr(0, line.size() - 1);
        parseCommand(line, fd, pollIdx);
    }
}
void    Server::runPoll() {
    // CRIA A ESTRUTURA DO DO SOCKET DO SERVIDOR PARA SER UTILIZADO NO POLL
    struct pollfd server;
    server.fd = _socketFd;
    server.events = POLLIN;
    server.revents = 0;
    _fds.push_back(server);

    log("Server ready and waiting for connections");
    while (_run)
    {
        int ret = poll(_fds.data(), _fds.size(), 0);   
        if (ret < 0 && _run)
            throw(std::runtime_error("Poll Error"));
        if (_fds[0].revents & POLLIN)
            connectNewClient();
        for (size_t i = 1; i < _fds.size(); i++)
        {  // Começa no índice 1, já que o índice 0 é o servidor
            if (_fds[i].revents & POLLHUP)
                disconnectClient(_fds[i].fd, i);
            if (_fds[i].revents & POLLIN)
                readNewMessage(i);
        }
    }
    log("Shutting down Server");
}

void	Server::sendResponse(int socket, const std::string &response) const {
    send(socket, response.c_str(), strlen(response.c_str()), 0);
}

void    Server::log(const std::string &logMessage) const {
    time_t timenow = time(NULL);
    tm*      localTime = localtime(&timenow);

    std::cout << GREEN << "[" << localTime->tm_mday << "-" << localTime->tm_mon + 1 << "-" << localTime->tm_year + 1900
    << " - " << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << "]" << RESET << " * " << logMessage  << std::endl;
}