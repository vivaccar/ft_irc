#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <vector>
#include <cstdlib>

class Server {
    private:
        int _port;
        std::string _password;
        int _socketFd;
        struct sockaddr_in _sockAddr;
        std::vector<struct pollfd> _fds;
        

    public:
        
        Server(const std::string &port, const std::string &password);
        ~Server();
        int getPort() const;
        const std::string &getPassword() const;

        void    createSocket();
        void    runPoll();
};