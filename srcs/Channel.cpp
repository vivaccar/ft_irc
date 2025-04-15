#include "../includes/Channel.hpp"

Channel::Channel(const std::string &name): _name(name), _mode("t") {}

Channel::~Channel() {}

std::string	Channel::getName() const {
	return this->_name;
}

std::string	Channel::getTopic() const {
	return this->_topic;
}

std::string	Channel::getPassword() const {
	return this->_password;
}

std::string	Channel::getMode() const {
	return this->_mode;
}

std::vector<int> Channel::getClients() const {
	return this->_channelClients;
}

std::vector<int> Channel::getAdmins() const {
	return this->_channelAdmins;
}

void	Channel::setName(const std::string &newName) {
	this->_name = newName;
}

void	Channel::setTopic(const std::string &newTopic) {
	this->_topic = newTopic;
}

void	Channel::setPassword(const std::string &newPass) {
	this->_password = newPass;
}

void	Channel::setMode(const std::string &newMode) {
	this->_mode = newMode;
}

void	Channel::addClient(const Client *client) {
	this->_channelAdmins.push_back(client->getSocket());
	client->getChannels().push_back(this);
}

void	Channel::addAdmin(const Client *client) {
	this->_channelAdmins.push_back(client->getSocket());
}