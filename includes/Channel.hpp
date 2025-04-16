#pragma once
#include "Client.hpp"

class Client; 
class Server; 

class Channel {
	private:
	std::string _name;
	std::string	_topic;
	std::string	_password;
	std::string _mode;
	std::vector<int> _channelClients;
	std::vector<int> _channelAdmins;
	//int		_userLimit;

	public:
		Channel(const std::string &name);
		~Channel();

		//GETTERS
		std::string	getName() const;
		std::string	getTopic() const;
		std::string	getPassword() const;
		std::string	getMode() const;
		std::vector<int> getClients() const;
		std::vector<int> getAdmins() const;

		//SETTERS
		void	setName(const std::string &newName);
		void	setTopic(const std::string &newTopic);
		void	setPassword(const std::string &newPass);
		void	setMode(const std::string &newMode);

		void	addClient(const Client *client);
		void	addAdmin(const Client *client);
};
