#include "Client.hpp"

class Server {
    private:
        int _port;
        std::string _password;
        int _socketFd;
        struct sockaddr_in _sockAddr;
        std::vector<struct pollfd> _fds;
        std::map<int, Client*>  _clients;

    public:
        
        Server(const std::string &port, const std::string &password);
        ~Server();
        int getPort() const;
        const std::string &getPassword() const;

        Client *getClient(int socket);
        void    createSocket();
        void    runPoll();
        void    createClient(int socket);
        void    parseCommand(std::string cmd, int clientSocket);
        void    checkPassword(std::vector<std::string> &cmds, Client *client);
        void    setNick(std::vector<std::string> &cmds, Client *client);
        void    setUser(std::vector<std::string> &cmds, Client *client);
};