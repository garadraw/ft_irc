#include <ctime>
#include <string>

#include "server.hpp"

void Server::pingClient()
{
    std::time_t timeNow;
    std::string stringPing;

    for (int i = 0; i < (int)_userList.size(); i++)
    {
        User & user = *_userList.at(i);
        stringPing = "PING :" + std::to_string(user.getTime()) + "\r\n";
        timeNow = std::time(NULL);
        int sendet = send(user.getFd(), stringPing.c_str(), sizeof(stringPing), 0);
        if (sendet == -1)
		{
			std::cerr << "Problem with Server-Ping" << std::endl;
			return;
		}
        // comparing sendet string to recieved string
        // If they are equal, comparing timestamps
        // if there difference is bigger than TIMEOUT -> killUser
        // if (string)
    }
}