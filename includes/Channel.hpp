#pragma once
#include "Client.hpp"

class Client; 
class Server; 

class Channel {
	private:
	std::string				_name;
	std::string				_topic;
	std::string				_key;
	std::string 			_mode;
	std::map<char, bool>	_modes;
	std::vector<int>		_channelClients;
	std::vector<int>		_channelAdmins;
	bool					_inviteOnly;
	bool					_topicRestricted;
	int						_userLimit;
/* 	bool				_hasUserLimit;
	bool				_hasPassword;
	int					_userLimit; */

	public:
		Channel(const std::string &name, const std::string &key);
		~Channel();

		//GETTERS
		std::string	getName() const;
		std::string	getKey() const;
		std::string	getTopic() const;
		std::string	getMode() const;
		std::vector<int> getClients() const;
		std::vector<int> getAdmins() const;
		std::vector<int> &getClientsRef();
		bool			getInviteOnly() const;
		bool			getTopicRestricted() const;

	
		//SETTERS
		void	setName(const std::string &newName);
		void	setTopic(const std::string &newTopic);
		void	setKey(const std::string &newKey);
		void	setMode(const std::string &newMode);
		void	setInviteOnly(bool status);
		void	setTopicRestricted(bool status);

		void	addClient(const Client *client);
		void	addAdmin(const Client *client);
};
