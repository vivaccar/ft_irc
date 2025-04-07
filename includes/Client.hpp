#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <vector>
#include <cstdlib>
#include <map>

class Client {
    private:
        int _socket;
        bool _isAuth;
        bool _insertPassword;
        std::string _nick;
        std::string _user;

    public:
        Client(const int socket);
        ~Client();

        void setAuth(bool status);
        void setInsertPassword(bool status);
        void setNick(const std::string &nick);
        void setUser(const std::string &user);
};