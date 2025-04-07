#include "../includes/Client.hpp"

Client::Client(int socket) : _socket(socket), _isAuth(false), _insertPassword(false) {
    std::cout << "New cliente created Socket " << _socket << std::endl; 
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
