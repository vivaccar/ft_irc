#include "../includes/Server.hpp"
 
int main(int ac, char **av)
{

    if (ac != 3)
    {
        std::cout << "Invalid arguments. Try ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    try
    {
        Server server(av[1], av[2]);
        server.recSignal();
        server.createSocket();
        server.runPoll();
    }
    catch(const std::exception& e)
    {
		std::cerr << e.what() << '\n';
    }

}