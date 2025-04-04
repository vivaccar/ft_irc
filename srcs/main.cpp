#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "../includes/Server.hpp"

/* int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Invalid arguments. Try ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    Server server(av[1], av[2]);

} */

/* #define PORT 6667

int main() {
    // Criar o socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Server Socket Fail" << std::endl;
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Aceita qualquer endereço IP
    address.sin_port = htons(PORT);  // Porta 6666

    // Ligar o socket à porta 6666
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Falha ao fazer bind." << std::endl;
        return -1;
    }

    
    // Colocar o servidor em modo de escuta
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Falha ao escutar." << std::endl;
        return -1;
    }

    std::cout << "Servidor esperando por uma conexão na porta " << PORT << "..." << std::endl;

    // Aceitar uma conexão de cliente
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&address);
    if (new_socket < 0) {
        std::cerr << "Falha ao aceitar a conexão." << std::endl;
        return -1;
    }

    std::cout << "Cliente conectado!" << std::endl;

    // Ler dados enviados pelo cliente
    while (1) {
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        std::cout << "Mensagem recebida do cliente: " << buffer << std::endl;
    }

    // Fechar o socket
    close(new_socket);
    close(server_fd);
    return 0;
} */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <vector>

#define PORT 6666
#define MAX_CLIENTS 10

int main() {
    // Criar o socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Falha ao criar o socket." << std::endl;
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    // Ligar o socket à porta 6666
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Falha ao fazer bind." << std::endl;
        return -1;
    }

    // Colocar o servidor em modo de escuta
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        std::cerr << "Falha ao escutar." << std::endl;
        return -1;
    }

    std::cout << "Servidor esperando por conexões na porta " << PORT << "..." << std::endl;

    // Estrutura para monitorar os sockets
    std::vector<struct pollfd> fds;
    fds.push_back({server_fd, POLLIN, 0});  // Adiciona o servidor para monitorar por novas conexões

    while (true) {
        // Esperar por eventos em qualquer socket
        int ret = poll(fds.data(), fds.size(), -1);  // Bloqueia até algum evento ocorrer
        if (ret < 0) {
            std::cerr << "Erro no poll." << std::endl;
            break;
        }

        // Verifica se o servidor está pronto para aceitar uma nova conexão
        if (fds[0].revents & POLLIN) {
            int new_socket = accept(server_fd, NULL, NULL);
            if (new_socket < 0) {
                std::cerr << "Falha ao aceitar a conexão." << std::endl;
                continue;
            }

            // Adiciona o novo cliente à lista de sockets monitorados
            std::cout << "Novo cliente conectado!" << std::endl;
            fds.push_back({new_socket, POLLIN, 0});
        }

        // Verifica cada socket de cliente para ver se há dados prontos para leitura
        for (size_t i = 1; i < fds.size(); ++i) {  // Começa no índice 1, já que o índice 0 é o servidor
            if (fds[i].revents & POLLIN) {
                char buffer[1024] = {0};
                int client_socket = fds[i].fd;
                int bytes_read = read(client_socket, buffer, sizeof(buffer));
                if (bytes_read == 0) {
                    // Cliente desconectou
                    std::cout << "Cliente desconectado." << std::endl;
                    close(client_socket);
                    fds.erase(fds.begin() + i);  // Remove o cliente da lista
                    --i;  // Ajusta o índice após a remoção
                } else {
                    std::cout << "Mensagem recebida do cliente: " << buffer << std::endl;

                    // Envia resposta ao cliente
                    const char* response = "Mensagem recebida com sucesso!";
                    send(client_socket, response, strlen(response), 0);
                }
            }
        }
    }
    close(server_fd);  // Fechar o socket do servidor
    return 0;
}