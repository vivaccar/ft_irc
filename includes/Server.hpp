#pragma once
#include "Client.hpp"
#include "Channel.hpp"

#define ERR_NEEDMOREPARAMS "461 ERROR: Need more parameters\n"
#define ERR_PASSWDMISMATCH "464 ERROR: Invalid password\n"
#define ERR_ALREADYREGISTERED "462 ERROR: Already Registered\n"
#define ERR_NOTREGISTERED "451 ERROR: You have not Registered\n"
#define ERR_NONICKNAMEGIVEN "431 ERROR: No Nickname Given\n"

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

        void    createSocket();
        void    runPoll();
        void	createClient(int socket);

		//COMMANDS
        void    parseCommand(std::string cmd, int clientSocket);
        void    checkPassword(std::vector<std::string> &cmds, Client *client);
        void    setNick(std::vector<std::string> &cmds, Client *client);
        void    setUser(std::vector<std::string> &cmds, Client *client);
		//commands
		int		kickUser(std::vector<std::string> &cmds, Client *client);
        void    joinCommand(std::vector<std::string> &cmds, Client *client);
        void    privMsg(std::vector<std::string> &cmds, Client *client);
};