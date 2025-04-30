#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define RED "\e[31m"
#define BLUE "\e[34m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"

#define BOLD "\e[1m"
#define BLINK "\e[5m"
#define RESET "\e[0m"


void send_line(int sock, const std::string& msg) {
    std::string full_msg = msg + "\n";
    send(sock, full_msg.c_str(), full_msg.size(), 0);
}

std::string receive_response(int sock) {
    char buffer[4096];
    int len = recv(sock, buffer, sizeof(buffer)-1, 0);
    if (len <= 0) 
        return "";
    buffer[len] = '\0';
    return std::string(buffer);
}

void    testjoin(int sock)
{
    std::string response = "";
    std::string expected = "";
    
    std::cout << "JOIN 1 CHANNEL: \'JOIN #canal\' -> " RESET;
    send_line(sock, "JOIN #canal");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    response = receive_response(sock);
    expected = ":testuser JOIN #canal\r\n";

    if (response.find(expected))
        std::cout << GREEN << "[ok]" << RESET << std::endl;
    else
        std::cout << RED << "[error]" << RESET << std::endl;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6667); // Porta padrão do IRC
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        return 1;
    }

    send_line(sock, "PASS senha");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    send_line(sock, "NICK testuser");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    send_line(sock, "USER testuser 0 * :Test User");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    testjoin(sock);
    return 0;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::string response = receive_response(sock);
    std::cout << "Resposta:\n" << response << std::endl; 

    if (response.find("#canal") != std::string::npos) {
        std::cout << "[OK] Servidor respondeu corretamente ao login.\n";
    } else {
        std::cerr << "[ERRO] Resposta inesperada do servidor.\n";
    }
    close(sock);
    return 0;
}