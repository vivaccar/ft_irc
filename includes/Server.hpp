#pragma once
#include "Client.hpp"
#include "Channel.hpp"
#include "response.hpp"
#include "utils.hpp"
#include <csignal>
#include <ctime>
#include <errno.h>


class Server {
    private:

        int _run;
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
        static Server* instance;

		//GETTERS
        int getPort() const;
        const std::string &getPassword() const;
		std::map<int, Client*> getClientsMap() const;
        Client *getClientBySocket(int socket);
		Client* getClientByNick(const std::string &nick);
        int     servRunning() const;
        Channel *getChannelByName(const std::string &name);

        void    createSocket();
        void    runPoll();
        void    connectNewClient();
        void    readNewMessage(size_t &pollIdx);
        void	createClient(int socket, std::string &hostname);
        void    disconnectClient(int fd, size_t &pollIdx);
        void    recSignal();
        static void    signalHandler(int signal);

		//COMMANDS
        void    parseCommand(std::string cmd, int clientSocket);
        void    checkPassword(std::vector<std::string> &cmds, Client *client);
        void    setNick(std::vector<std::string> &cmds, Client *client);
        bool	nickColission(std::string &nick);
        void    setUser(std::vector<std::string> &cmds, Client *client);
		
        //commands
		int		kickUser(std::vector<std::string> &cmds, Client *client);
		int		inviteUser(std::vector<std::string> &cmds, Client *client);
        void    joinCommand(std::vector<std::string> &cmds, Client *client);
        void    privMsg(std::vector<std::string> &cmds, Client *client);
        void    topic(std::vector<std::string> &cmds, Client *client, std::string cmd);
        void    mode(std::vector<std::string> &cmds, Client *client, std::string cmd);
        void    parseModeCommands(std::vector<std::string>& cmds, Client* client, Channel *channel);
        void    executeModeCommands(std::string action, std::vector<std::string>& cmds, unsigned int &parameter, Client* client, Channel *channel);


        //RESPONSE
        void	sendResponse(int socket, const std::string &response) const;

        void    log(const std::string &logMessage) const;
};

// UTILS

std::string					extractMessage(std::string cmd, int maxSpaces);
std::vector<std::string>	splitString(std::string &str);
