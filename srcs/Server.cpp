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

void    Server::createServerSocket() {
    
}

