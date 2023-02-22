#include <ctime>
#include <string>

#include "server.hpp"
#include "responses.hpp"

void Server::pingClient()
{
	std::time_t timeNow;
	std::string stringPing;

	for (int i = 0; i < (int)_userList.size(); i++)
	{
		User & user = *_userList.at(i);
		stringPing = "PING :" + std::to_string(user.getTime()) + "\r\n";
		timeNow = std::time(NULL);
		int sendet = send(*(user.getFd()), stringPing.c_str(), sizeof(stringPing), 0);
		if (sendet == -1)
		{
			std::cerr << "Problem with Server-Ping" << std::endl;
			return;
		}
        timeNow = std::time(NULL);
        // timestamps of now and the response of a user
        if (user.getTime() - timeNow > TIMEOUT)
        {
            // if the difference is bigger than TIMEOUT -> killUser
            std::cout << user.getNick() << " has been killed caused by ping timeout" << std::endl;
           //killUser(user, "ping timeout");
        }
        
    }
}