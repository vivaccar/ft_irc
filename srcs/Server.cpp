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
            struct pollfd newClient;
            newClient.fd = newClientSock;
            newClient.events = POLLIN;
            newClient.revents = 0;
            _fds.push_back(newClient);
            std::cout << "New client connected on FD: " << newClient.fd << std::endl;
        }
        for (size_t i = 1; i < _fds.size(); i++)
        {  // Começa no índice 1, já que o índice 0 é o servidor
            if (_fds[i].revents & POLLIN)
            {
                char buffer[1024] = {0};
                int client_socket = _fds[i].fd;
                int bytes_read = read(client_socket, buffer, sizeof(buffer));
                if (bytes_read == 0)
                {
                    // Cliente desconectou
                    std::cout << "Client desconected on FD" << _fds[i].fd << std::endl;
                    close(client_socket);
                    _fds.erase(_fds.begin() + i);  // Remove o cliente da lista
                    --i;  // Ajusta o índice após a remoção
                } else
                {
                    std::cout << "Client " << _fds[i].fd << " say:" << buffer << std::endl;
                    // Envia resposta ao cliente
                    const char* response = "Message received by the server!";
                    send(client_socket, response, strlen(response), 0);
                }
            }
        }
    }
}
