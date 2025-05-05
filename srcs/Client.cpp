#include "../includes/Client.hpp"

Client::Client(int socket, std::string &hostname) : _socket(socket), _isAuth(false), _insertPassword(false), _hostname(hostname) {
    //std::cout << "New client created Socket " << _socket << std::endl; 
}

Client::~Client() {
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		Channel *channel = *it;
		channel->removeClient(this->getSocket());
		if (isChannelAdmin(channel))
			channel->removeAdmin(this->getSocket());
	}
}

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

std::string		Client::getRealName() const {
	return this->_realName;
}

std::string		Client::getHostname() const {
	return this->_hostname;
}

std::vector<Channel *> Client::getChannels() const {
	return this->_channels;
}

const std::string Client::getPrefix() const {
	return _nick + "!" + _user + "@" + _hostname;
}
//:dan!d@localhost
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

void	Client::setRealName(const std::string &realName) {
	this->_realName = realName;
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

void	Client::sendToChannel(Channel *channel, const std::string &msg) {
	std::vector<int> members = channel->getClients();
	for (std::vector<int>::iterator it = members.begin(); it != members.end(); it++) {
		if (*it == this->getSocket())
			continue ;
		send(*it, msg.c_str(), msg.size(), 0);
	}
}

void	Client::sendToAllChannel(Channel *channel, const std::string &msg) {
	std::vector<int> members = channel->getClients();
	for (std::vector<int>::iterator it = members.begin(); it != members.end(); it++) {
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

bool	Client::isChannelInvited(Channel *channel) {
	std::vector<int> members = channel->getChannelInvites();
	if (std::find(members.begin(), members.end(), this->getSocket()) != members.end())
		return true;
	return false;
}