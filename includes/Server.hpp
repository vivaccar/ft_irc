#pragma once
#include "Client.hpp"
#include "Channel.hpp"
#include <csignal>

#define ERR_NEEDMOREPARAMS(nick, cmd) ": 461 " + nick + " " + cmd + " :Need more parameters\r\n"
#define ERR_PASSWDMISMATCH(nick) ": 464 " + nick + " : Invalid password\r\n"
#define ERR_ALREADYREGISTERED(nick) ": 462 " + nick + " :You may not reregister\r\n"
#define ERR_NOTREGISTERED(nick) ": 451 " + nick + " :You have not Registered\r\n"
#define ERR_NONICKNAMEGIVEN(nick) ": 431 " + nick + " :No Nickname Given\r\n"
#define ERR_NICKNAMEINUSE(nick, attempt) ": 433 " + nick + " " + attempt + " :Nickname in use.\r\n"
#define ERR_ERRONEUSNICKNAME(nick, attempt) ": 432 " + nick + " " + attempt + ": Erroneus nickname\r\n"
#define ERR_UNKNOWNCOMMAND(nick, cmd) ": 421 " + nick + " " + cmd + " :Unknown command\r\n"

#define WELCOME(nick) ": 001 " + nick + " :Welcome " + nick + " to the ft_irc\r\n"


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

        void    createSocket();
        void    runPoll();
        void	createClient(int socket);
        void    recSignal();
        static void    signalHandler(int signal);

		//COMMANDS
        void    parseCommand(std::string cmd, int clientSocket);
        void    checkPassword(std::vector<std::string> &cmds, Client *client);
        void    setNick(std::vector<std::string> &cmds, Client *client);
        bool	nickColission(std::string &nick);
        void    setUser(std::vector<std::string> &cmds, Client *client);
        void    joinCommand(std::vector<std::string> &cmds, Client *client);
        void    privMsg(std::vector<std::string> &cmds, Client *client);

        //RESPONSE
        void	sendResponse(int socket, const std::string &response) const;
};
