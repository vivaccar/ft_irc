#include "../includes/Client.hpp"

Client::Client(int socket) : _socket(socket), _isAuth(false), _insertPassword(false) {
    std::cout << "New client created Socket " << _socket << std::endl; 
}

Client::~Client() {}

void    Client::setInsertPassword(bool status) {
    _insertPassword = status;
}

void    Client::setAuth(bool status) {
    _isAuth = status;
}

void    Client::setNick(const std::string &nick) {
    this->_nick = nick;
}

void    Client::setUser(const std::string &user) {
    this->_user = user;
}

int     Client::getSocket() const {
    return this->_socket;
}

std::string     Client::getUser() const {
    return this->_user;
}

std::string     Client::getNick() const {
    return this->_nick;
}

bool    Client::isAuth() const {
    return this->_isAuth;
}

bool    Client::insertPassword() const {
    return this->_insertPassword;
}