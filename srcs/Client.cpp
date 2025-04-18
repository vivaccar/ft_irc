#include "../includes/Client.hpp"

Client::Client(int socket) : _socket(socket), _isAuth(false), _insertPassword(false) {
    //std::cout << "New client created Socket " << _socket << std::endl; 
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

Channel *Client::createChannel(const std::string &name) {
	Channel *c = new Channel(name);
	c->addClient(this);
	c->addAdmin(this);
	this->_channels.push_back(c);
	return c;
}

void	Client::sendToChannel(Channel *channel, std::string &msg) {
	std::vector<int> members = channel->getClients();
	for (std::vector<int>::iterator it = members.begin(); it != members.end(); it++) {
		if (*it == this->getSocket())
			continue ;
		send(*it, msg.c_str(), msg.size(), 0);
	}
}

void	Client::sendToClient(Client *client, const std::string &msg) {
	send(client->getSocket(), msg.c_str(), msg.size(), 0);
}

void	Client::sendError(Client *client, const char *error) {
	send(client->getSocket(), error, strlen(error), 0);
}

bool	Client::isChannelMember(Channel *channel) {
	std::vector<int> members = channel->getClients();
	if (std::find(members.begin(), members.end(), this->getSocket()) != members.end())
		return true;
	return false;
}

bool	Client::isChannelAdmin(Channel *channel) {
	std::vector<int> admins = channel->getAdmins();
	if (std::find(admins.begin(), admins.end(), this->getSocket()) != admins.end())
		return true;
	return false;
}