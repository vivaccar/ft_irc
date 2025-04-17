#pragma once
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
#include <algorithm>
#include "Channel.hpp"
#include <fcntl.h>

#define RED "\e[31m"
#define BLUE "\e[34m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"

#define BOLD "\e[1m"
#define BLINK "\e[5m"
#define RESET "\e[0m"

class Channel;

class Client {
    private:
            int _socket;
            bool _isAuth;
            bool _insertPassword;
            std::string _nick;
            std::string _user;
            std::vector<Channel *> _channels;
        
        public:
            Client(const int socket);
            ~Client();
            //GETTERS    
            bool passInserted() const;
            int  getSocket() const;
            std::string getUser() const;
            std::string getNick() const;
            std::vector<Channel *> getChannels() const;
            //SETTERS
            void setAuth(bool status);
            void setInsertPassword(bool status);
            void setNick(const std::string &nick);
            void setUser(const std::string &user);
            
            bool isAuth() const;
            bool insertPassword() const;
            Channel *createChannel(const std::string &name);
            int	joinChannel(Channel *channel);
};
