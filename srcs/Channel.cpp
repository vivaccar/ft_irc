#include "../includes/Channel.hpp"

Channel::Channel(const std::string &name, const std::string &key): _name(name), _key(key), _mode("t"),
_inviteOnly(false), _topicRestricted(false), _userLimit(-1)/*  _hasPassword(0), _hasUserLimit(0), _userLimit(0) */ {
	(void) _userLimit;
}

Channel::~Channel() {}

std::string	Channel::getName() const {
	return this->_name;
}

std::string	Channel::getTopic() const {
	return this->_topic;
}

std::string	Channel::getKey() const {
	return this->_key;
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

std::vector<int> & Channel::getClientsRef(){
	return (this->_channelClients);
}

bool	Channel::getInviteOnly() const {
	return this->_inviteOnly;
}

bool	Channel::getTopicRestricted() const {
	return this->_topicRestricted;
}

void	Channel::setName(const std::string &newName) {
	this->_name = newName;
}

void	Channel::setTopic(const std::string &newTopic) {
	this->_topic = newTopic;
}

void	Channel::setKey(const std::string &newKey) {
	this->_key = newKey;
}

void	Channel::setMode(const std::string &newMode) {
	this->_mode = newMode;
}

void	Channel::setInviteOnly(bool status) {
	this->_inviteOnly = status;
}

void	Channel::setTopicRestricted(bool status) {
	this->_topicRestricted = status;
}

void	Channel::addClient(const Client *client) {
	this->_channelClients.push_back(client->getSocket());
	client->getChannels().push_back(this);
}

void	Channel::addAdmin(const Client *client) {
	this->_channelAdmins.push_back(client->getSocket());
}