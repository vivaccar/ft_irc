#include "../../includes/Channel.hpp"

std::string extractMessage(std::string cmd, int maxSpaces)
{
    int index = 0;
    int spaces = 0;

    while (std::isspace(cmd[index]))
        index++;
    while (spaces < maxSpaces) 
    {
        if (std::isspace(cmd[index]))
        {
            while (std::isspace(cmd[index]))
                index++;
            spaces++;
        }
        else 
            index++;
    }
    return cmd.substr(index + 1, cmd.size());
}