#include "Client.hpp"
#include "Channel.hpp"

class Server {
    private:
        int _port;
        int _socketFd;
        std::string _password;
        struct sockaddr_in _sockAddr;
        std::vector<struct pollfd> _fds;
        std::map<std::string, Client*>  _clients;
        std::map<std::string, Channel*>  _channels;

    public:
        
        Server(const std::string &port, const std::string &password);
        ~Server();
        int getPort() const;
        const std::string &getPassword() const;

        Client *getClient(int socket);
        void    createSocket();
        void    runPoll();
        void    addNewClient(Client *client);

		//COMMANDS
        void    parseCommand(std::string cmd, int clientSocket);
        void    checkPassword(std::vector<std::string> &cmds, Client *client);
        void    setNick(std::vector<std::string> &cmds, Client *client);
        void    setUser(std::vector<std::string> &cmds, Client *client);
        void    joinCommand(std::vector<std::string> &cmds, Client *client);
        void    privMsg(std::vector<std::string> &cmds, Client *client);
};