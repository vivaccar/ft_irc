#include "Client.hpp"


class Channel {
	private:
	std::string _name;
	std::string	_topic;
	std::vector<int> _channelClients;
	std::vector<int> _channelAdmins;

	public:
		Channel(const std::string name);
		~Channel();
}
