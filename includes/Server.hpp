#pragma once
#include "Client.hpp"
#include "Channel.hpp"

class Server {
    private:
        int _port;
        int _socketFd;
        std::string _password;
        struct sockaddr_in _sockAddr;
        std::vector<struct pollfd> _fds;
        std::map<int, Client*>  _clients;
        std::map<std::string, Channel*>  _channels;

    public:
        
        Server(const std::string &port, const std::string &password);
        ~Server();

		//GETTERS
        int getPort() const;
        const std::string &getPassword() const;
		std::map<int, Client*> getClientsMap() const;
        Client *getClientBySocket(int socket);
		Client* getClientByNick(const std::string &nick);
        Channel *getChannelByName(const std::string &name);

        void    createSocket();
        void    runPoll();
        void	createClient(int socket);

		//COMMANDS
        void    parseCommand(std::string cmd, int clientSocket);
        void    checkPassword(std::vector<std::string> &cmds, Client *client);
        void    setNick(std::vector<std::string> &cmds, Client *client);
        void    setUser(std::vector<std::string> &cmds, Client *client);
        void    joinCommand(std::vector<std::string> &cmds, Client *client);
        void    privMsg(std::vector<std::string> &cmds, Client *client);
};

#define ERR_NEEDMOREPARAMS "461 ERROR: Not enough parameters\n"
#define ERR_BANNEDFROMCHAN "474 ERROR: You are banned from this channel\n"
#define ERR_INVITEONLYCHAN "473 ERROR: Invite-only channel\n"
#define ERR_BADCHANNELKEY "476 ERROR: Wrong password\n"
#define ERR_CHANNELISFULL "471 ERROR: Channel is full\n"
#define ERR_BADCHANMASK "476 ERROR: Bad Channel Mask\n"
#define ERR_NOSUCHCHANNEL "403 ERROR: No such channel\n"
#define ERR_TOOMANYCHANNELS "405 ERROR: You have joined too many channels\n"
