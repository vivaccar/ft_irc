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
    return cmd.substr(index, cmd.size());
}

std::vector<std::string> splitString(std::string &str) {
	size_t start = 0;
	size_t end = 0;
	std::vector<std::string> vec;
	while(end != str.size()) {
		end = str.find(",", start);
		if (end == str.npos)
			end = str.size();
		vec.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	return vec;
}