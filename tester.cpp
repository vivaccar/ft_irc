#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "responseTest.hpp"
#include <sstream>


#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

int connect()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6667); // Porta padrão do IRC
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        return 1;
    }
    return sock;
}

void send_line(int sock, const std::string& msg) {
    std::string full_msg = msg + "\n";
    send(sock, full_msg.c_str(), full_msg.size(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

std::string receive_response(int sock) {
    char buffer[4096];
    bzero(buffer, 4096);
    int len = recv(sock, buffer, sizeof(buffer)-1, 0);
    if (len <= 0) 
        return "";
    buffer[len] = '\0';
    return std::string(buffer);
}

void makeTest(int sock, std::string command, std::string testName, std::string expected)
{
    send_line(sock, command);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    std::string resp = receive_response(sock);

    std::cout << CYAN << "\nTEST NAME: " << RESET << YELLOW << testName <<  " - " << resp << RESET << std::endl;
/*     std::cout << "Command: " << command << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    std::cout << "Received: " << resp; */

    std::cout << "Result: ";
    if (resp.find(expected) != std::string::npos) {
        std::cout << GREEN << "[OK]" << RESET << std::endl;
    } else {
        std::cerr << RED << "[ERRO]" << RESET << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}


void    testChannel(int sock)
{
    std::cout << RED << "\nTEST CASE [05] - CHANNEL TESTS\n" << RESET << std::endl; 
    
    makeTest(sock, "JOIN", "1 - JOIN NEED MORE PARAMS", "461");
    makeTest(sock, "JOIN ", "2 - JOIN NEED MORE PARAMS", "461");
    makeTest(sock, "  JOIN ", "3 - JOIN NEED MORE PARAMS", "461");
    makeTest(sock, "JOIN INVALID", "4 - JOIN INVALID CHANNEL NAME", "476");

    // CREATING A CHANNEL TO TEST CALLED #42.
    send_line(sock, "JOIN #42");
    // CREATING NEW USER
    int newUser = connect();
    // AUTH NEW USER
    send_line(newUser, "PASS senha\nNICK newUser\nUSER NEWUSER 0 0 0");
    // SET INVITE ONLY AND PASSWORD;
    makeTest(sock, "MODE #42 k senha", "5 - SETTING KEY TO CHANNEL", "MODE #42 +k :senha");
    makeTest(sock, "MODE #42 -k senha", "6 - REMOVING CHANNEL KEY", "MODE #42 :-k");
    makeTest(sock, "MODE #42 +k senha", "7 - SETTING KEY TO CHANNEL", "MODE #42 +k :senha");
    makeTest(sock, "MODE #42 +k", "8 - MISSING KEY PARAM", "696");
    makeTest(sock, "MODE #42 i senha", "9 - SETTING INVITE ONLY CHANNEL", "MODE #42 :+i");
    makeTest(sock, "MODE #42 -i senha", "10 - REMOVING INVITE ONLY CHANNEL", "MODE #42 :-i");
    makeTest(sock, "MODE #42 +i senha", "11 - SETTING INVITE ONLY CHANNEL", "MODE #42 :+i");
    makeTest(sock, "MODE #42 -k senha", "12 - REMOVING CHANNEL KEY", "MODE #42 :-k");
    
    // MODE NO SUCH CHANNEL
    makeTest(sock, "MODE #invalid +k senha", "13 - SETTING K INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid +l 10", "14 - SETTING L INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid +t", "15 - SETTING T INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid +o USER", "16 - SETTING O INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid +i", "17 - SETTING I INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid -k", "18 - SETTING -K INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid -l", "19 - SETTING -L INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid -t", "20 - SETTING -T INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid -o", "21 - SETTING -O INVALID CHANNEL", "403");
    makeTest(sock, "MODE #invalid -i", "22 - SETTING -I INVALID CHANNEL", "403");
    

    // JOIN INVITE ONLY CHANNEL 
    makeTest(newUser, "JOIN #42 senha", "22 - JOIN INVITE ONLY CHANNEL", "473");
    // JOIN INVALID CHANNEL
    makeTest(newUser, "JOIN invalid", "23 - JOIN INVALID CHANNEL", "476");


    close(newUser);





    //" MODE " + channel + " " + cmd + " :" + key
    // 
}


void    testPass(int sock)
{
    makeTest(sock, "PASS senh", "7 - SEND INVALID PASSWORD", "464");
    makeTest(sock, "PASS", "8 - NEED MORE PARAMS", "461");
}

void    testAuth(int sock)
{
    /* TEST ALL POSSIBLE COMMANDS BEFORE AUTHENTICATION */
    std::cout << RED << "TEST CASE [01] - SENDING COMMANDS BEFORE PASS\n" << RESET << std::endl; 
    
    makeTest(sock, "NICK vinicius", "1 - NICK BEFORE PASS", "451 :You have not Registered");
    makeTest(sock, "USER vinicius 0 0 0", "2 - USER BEFORE PASS", "451 :You have not Registered");
    makeTest(sock, "INVITE #channel nick", "3 - INVITE BEFORE PASS", "451 :You have not Registered");
    makeTest(sock, "KICK #channel", "4 - KICK BEFORE PASS", "451 :You have not Registered");
    makeTest(sock, "PRIVMSG #NICK :msg", "5 - PRIVMSG BEFORE PASS", "451 :You have not Registered");
    makeTest(sock, "TOPIC #canal", "6 - TOPIC BEFORE PASS", "451 :You have not Registered");
    testPass(sock);
    
    send_line(sock, "PASS senha");
    std::cout << RED << "\nTEST CASE [02] - SENDING COMMANDS AFTER PASS, BEFORE AUTH\n" << RESET << std::endl;
    
    makeTest(sock, "INVITE #channel nick", "1 - INVITE BEFORE AUTH", "451 :You have not Registered");
    makeTest(sock, "KICK #channel", "2 - KICK BEFORE AUTH", "451 :You have not Registered");
    makeTest(sock, "PRIVMSG #NICK :msg", "3 - PRIVMSG BEFORE AUTH", "451 :You have not Registered");
    makeTest(sock, "TOPIC #canal", "4 - TOPIC BEFORE AUTH", "451 :You have not Registered");
    makeTest(sock, "PASS senha", "5 - SEND PASS AFTER PASS ALREADY INSERTED", "462");
    makeTest(sock, "USER testuser 0 0", "6 - USER NEED MORE PARAMS", "461");
    makeTest(sock, "USER testuser ", "7 - USER NEED MORE PARAMS", "461");
    makeTest(sock, "USER testuser 0", "8 - USER NEED MORE PARAMS", "461");
    makeTest(sock, "USER", "9 - USER NEED MORE PARAMS", "461");
    send_line(sock, "NICK testuser");
    makeTest(sock, "USER testuser 0 0 0", "10 - AUTH COMPLETED", "001");
    makeTest(sock, "USER vinicius 0 0 0", "11 - USER AFTER AUTH COMPLETED", "462");
}

void    testNick(int sock)
{
    std::cout << RED << "\nTEST CASE [03] - NICK COMMAND\n" << RESET << std::endl; 

    makeTest(sock, "NICK vinicius", "1 - VALID NICKNAME", "NICK vinicius");
    makeTest(sock, "NICK vinicius", "2 - NICKNAME IN USE ", "433");
    makeTest(sock, "NICK", "3 - NO NICKNAME GIVEN", "431");
    makeTest(sock, "NICK 12", "4 - ERRONEUS NICK", "432");
    makeTest(sock, "NICK AKSJDKASJDKA", "5 - ERRONEUS NICK", "432");
    makeTest(sock, "NICK -LSL", "6 - ERRONEUS NICK", "432");
    makeTest(sock, "NICK 1VINICIUS", "7 - ERRONEUS NICK", "432");
    makeTest(sock, "NICK {TESTUSER}", "8 - ERRONEUS NICK", "432");
    makeTest(sock, "NICK $", "9 - ERRONEUS NICK", "432");
}


void makeclientstest(std::string testName, int sender, std::string cmd, int receiver, std::string expected)
{
    send_line(sender, cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::string resp = receive_response(receiver);

    std::cout << CYAN << "\nTEST NAME: " << RESET << YELLOW << testName << RESET << std::endl;
/*     std::cout << "Comand: " << cmd << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    std::cout << "Received: " << resp; */
    std::cout << "Result: ";
    if (resp.find(expected) != std::string::npos) {
        std::cout << GREEN << "[OK]" << std::endl << RESET;
    } else {
        std::cerr << RED << "[ERRO]" << std::endl << RESET;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}
void    testclients()
{
    std::cout << RED << "\nTEST CASE [06] - CLIENTS INTERACTION\n" << RESET << std::endl; 
    
    int vini = connect();
    int mano = connect();

    send_line(vini, "PASS senha\nNICK vini\nUSER Vini * 0 vinicius");
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    send_line(mano, "PASS senha\nNICK mano\nUSER Mano * 0 mano");
    std::this_thread::sleep_for(std::chrono::milliseconds(150));


    send_line(mano, "JOIN #students");
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    makeclientstest("1 - NEW CLIENT ON CHANNEL", vini, "JOIN #students", mano, "vini JOIN #students");
    makeclientstest("2 - SEND MESSAGE ON CHANNEL", vini, "PRIVMSG #students :ola galera", mano, "vini PRIVMSG #students");
    makeclientstest("3 - SEND PRIVATE MESSAGE", vini, "PRIVMSG mano :ola mano", mano, "vini PRIVMSG mano");
    makeclientstest("4 - SEND PRIVATE MESSAGE", mano, "PRIVMSG vini :ola vini", vini, "mano PRIVMSG vini");
    
    // TOPIC
    makeTest(vini, "TOPIC #students", "5 - NO TOPIC SET",  "331");
    makeclientstest("6 - SET NEW TOPIC", mano, "TOPIC #students :alterei o topico", vini, "TOPIC #students alterei o topico");
    makeTest(vini, "TOPIC #students", "7 - VIEW TOPIC", "332");
    makeTest(vini, "TOPIC #students", "8 - VIEW TOPIC", "332");

    // MODE
    makeTest(mano, "mode #students +i" , "9 - MODE INVITE ONLY", "MODE #students :+i");
    makeTest(mano, "mode #students +t" , "10 - MODE TOPIC RESTRICTED", "MODE #students :+t");
    makeTest(mano, "mode #students +l 10" , "11 - MODE USER LIMIT", "MODE #students +l :10");
    makeTest(mano, "mode #students +k senha" , "12 - MODE CHANNEL PASS", "MODE #students +k :senha");
    makeTest(mano, "mode #students -i" , "13 - MODE REMOVE INVITE ONLY", "MODE #students :-i");
    makeTest(mano, "mode #students -t" , "14 - MODE REMOVE TOPIC RESTRICTED", "MODE #students :-t");
    makeTest(mano, "mode #students -l" , "15 - MODE REMOVE USER LIMIT", "MODE #students :-l");
    makeTest(mano, "mode #students -k" , "16 - MODE REMOVE KEY", "MODE #students :-k");
    makeTest(mano, "mode #students +o vini" , "17 - MODE ADD OP", "MODE #students +o :vini");
    makeTest(mano, "mode #students -o vini" , "18 - MODE REMOVE OP", "MODE #students -o :vini");
    
    // create new client to test invite.

    int celo = connect();
    send_line(celo, "PASS senha\nnick celo\nuser marcelo 0 0 0");


    // INVITE TESTS
    makeTest(vini, "INVITE name #students", "19 - INVITE NO SUCH NICK", "442");
    makeTest(vini, "INVITE o", "20 - INVITE NO PARAMS", "461");
    makeTest(vini, "INVITE ", "22 - INVITE NO PARAMS", "461");
    makeTest(vini, "INVITE mano #invalid", "23 - INVITE NO SUCH CHANNEL", "442");
    makeTest(vini, "INVITE mano #students", "24 - INVITE ALREADY ON CHANNEL", "443");
    send_line(mano, "mode #students +i");
    makeTest(vini, "INVITE celo #students", "25 - NOT OPERATOR INVITE SOMEONE IN +I CHANNEL", "482");
    makeclientstest("26 - INVITE SOMEONE TO CHANNEL", mano, "invite celo #students", celo, "You have been invited to #students");
    makeTest(celo, "join #students", "27 - JOIN AFTER INVITE", ":celo JOIN #students");

    // KICK TESTS
    makeTest(mano, "kick ", "28 - KICK NEED MORE PARAMS", "461");
    makeTest(mano, "kick #students", "28 - KICK NEED MORE PARAMS", "461");






    //MODE " + channel + " :" + status

//TOPIC_CHANGE(nick, channel, topic) ":" + nick + " TOPIC " + channel + " " + topic + "\r\n"
    close(mano);
    close(vini);
    close(celo);

}

void checkleaks() {
    int a = connect();
    int b = connect();
    int c = connect();
    int d = connect();
    int e = connect();
    int f = connect();
    int g = connect();

    send_line(a, "PASS senha\r\nNICK a\r\nUSER a 0 0 0");
    send_line(b, "PASS senha\r\nNICK b\r\nUSER b 0 0 0");
    send_line(c, "PASS senha\r\nNICK c\r\nUSER c 0 0 0");
    send_line(d, "PASS senha\r\nNICK d\r\nUSER d 0 0 0");
    send_line(e, "PASS senha\r\nNICK e\r\nUSER e 0 0 0");
    send_line(f, "PASS senha\r\nNICK f\r\nUSER f 0 0 0");
    send_line(g, "PASS senha\r\nNICK g\r\nUSER g 0 0 0");

    send_line(a, "JOIN #1");
    send_line(a, "JOIN #2");
    send_line(a, "JOIN #3");

    send_line(b, "JOIN #1");
    send_line(c, "JOIN #1");
    send_line(d, "JOIN #2");
    send_line(e, "JOIN #2");
    send_line(f, "JOIN #3");
    send_line(g, "JOIN #3");

    send_line(a, "MODE #1 +i");
    send_line(a, "MODE #1 +k senha1");
    send_line(a, "MODE #1 +l 5");
    send_line(a, "MODE #1 +t");

    send_line(a, "INVITE f #1");
    send_line(f, "JOIN #1 senha1");

    send_line(b, "MODE #1 +o b");
    send_line(c, "MODE #1 +o c");
    send_line(d, "MODE #2 +o d");
    send_line(e, "MODE #2 +o e");

    send_line(f, "JOIN #4");
    send_line(f, "JOIN #5");
    send_line(g, "JOIN #5");

    send_line(f, "MODE #5 +i");
    send_line(f, "INVITE g #5");
    send_line(g, "JOIN #5");

    send_line(b, "TOPIC #1 :Bem-vindos ao canal 1");
    send_line(e, "TOPIC #2 :Topico canal 2");

    send_line(c, "KICK #1 b");
    send_line(d, "KICK #2 e");
    send_line(e, "KICK #2 d");

    close(b);
    close(c);
    close(d);
    close(e);

    int h = connect();
    send_line(h, "PASS senha\r\nNICK h\r\nUSER h 0 0 0");
    send_line(h, "JOIN #1 senha1");
    send_line(h, "JOIN #2");
    send_line(h, "JOIN #3");

    int i = connect();
    send_line(i, "PASS senha\r\nNICK i\r\nUSER i 0 0 0");
    send_line(i, "JOIN #1 senha1");
    send_line(i, "JOIN #5");

    send_line(i, "MODE #1 -k senha1");
    send_line(i, "MODE #1 -i");
    send_line(i, "MODE #1 -t");
    send_line(i, "MODE #1 -l");

    send_line(h, "MODE #1 +k nova");
    send_line(h, "INVITE i #1");

    send_line(f, "PART #4");
    send_line(f, "PART #5");

    send_line(g, "PART #5");

    close(a);
    close(f);
    close(g);
    close(h);
    close(i);

    // STRESS LOOP
    for (int j = 0; j < 20; ++j) {
        int sock = connect();
        std::stringstream nick;
        nick << "N" << j;
        std::stringstream user;
        user << "U" << j;
        std::stringstream channel;
        channel << "#chan" << j % 5;

        send_line(sock, "PASS senha");
        send_line(sock, "NICK " + nick.str());
        send_line(sock, "USER " + user.str() + " 0 0 0");
        send_line(sock, "JOIN " + channel.str());

        if (j % 2 == 0)
            send_line(sock, "MODE " + channel.str() + " +i");

        if (j % 3 == 0)
            send_line(sock, "TOPIC " + channel.str() + " :Stress test");

        close(sock);
    }
}

int main() {
/*     int sock = connect();

    testAuth(sock);
    testNick(sock);
    testChannel(sock);
    
    close(sock);
    testclients(); */
    checkleaks();
    return 0;
}