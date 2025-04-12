#include "../includes/Client.hpp"

Client::Client(int socket) : _socket(socket), _isAuth(false), _insertPassword(false) {
    std::cout << "New client created Socket " << _socket << std::endl; 
}

Client::~Client() {}

// - - - - - - - GETTERS - - - - - - - 
int     Client::getSocket() const {
    return this->_socket;
}

bool    Client::passInserted() const {
    return this->_insertPassword;
}

std::string     Client::getUser() const {
    return this->_user;
}

std::string     Client::getNick() const {
    return this->_nick;
}

std::vector<Channel *> Client::getChannels() const {
	return this->_channels;
}

// - - - - - - - SETTERS - - - - - - - 
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

bool    Client::isAuth() const {
    return this->_isAuth;
}

bool    Client::insertPassword() const {
    return this->_insertPassword;
}

Channel *Client::createChannel(const std::string &name) {
	Channel *c = new Channel(name);
	c->addClient(this);
	c->addAdmin(this);
	this->_channels.push_back(c);
	return c;
}

int	Client::joinChannel(Channel *channel) {
	//SE O CLIENTE JA FIZER PARTE DO CANAL, NADA ACONTECE
	if (std::find(channel->getClients().begin(), channel->getClients().end(), this->getSocket()) == channel->getClients().end()) {
		channel->addClient(this);
		//MENSAGEM DE BOAS VINDAS
		std::string msg = "Welcome to channel " + channel->getName() + "\nTopic: " + channel->getTopic() + "\n"; 
		send(this->getSocket(), msg.c_str(), msg.size(), 0);
		//DESENVOLVER MENSAGEM DE JOIN PARA OUTROS MEMBROS DO CANAL
		return 1;
	}
	//desenvolver outras possibilidades, como:
	//tentar entrar num canal que esta mode invite-only, sem convite;
	//verificar se canal tem password;
	//verificar se o canal tem users limit;
	return 0;
		
	

}

