#include <iostream>
#include <string>
#include <cstdlib>

class Server {
    private:
        int _port;
        std::string _password;
        //int _socketFd;

    public:
        
        Server(const std::string &port, const std::string &password);
        ~Server();
        int getPort() const;
        const std::string &getPassword() const;

        void    createServerSocket();
        
};