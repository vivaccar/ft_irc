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
            std::string _realName;
            std::vector<Channel *> _channels;
        
        public:
            Client(const int socket);
            ~Client();

            //GETTERS    
            bool passInserted() const;
            int  getSocket() const;
            std::string getUser() const;
            std::string getNick() const;
            std::string getRealName() const;
            std::vector<Channel *> getChannels() const;
            bool isAuth() const;
            

            //SETTERS
            void setAuth(bool status);
            void setInsertPassword(bool status);
            void setNick(const std::string &nick);
            void setUser(const std::string &user);
            void setRealName(const std::string &realName);
            
            Channel *createChannel(const std::string &name);
            int	    joinChannel(Channel *channel);


            void	sendToChannel(Channel *channel, std::string &msg);
            void	sendToClient(Client *client, const std::string &msg);
            void	sendError(Client *client, const char *error);
            bool	isChannelMember(Channel *channel);
            bool	isChannelAdmin(Channel *channel);
};
