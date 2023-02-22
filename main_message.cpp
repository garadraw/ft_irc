#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <poll.h>
#include <sstream>
#include <signal.h>

#include "server.hpp"
#include "Message.hpp"

int main(int argc, char** argv)
{
	std::string	sample_input = "/ PiNg         ";
	Message	*test = new Message(sample_input);

	if (sample_input.at(0) == '/' && sample_input.at(1) != ' ')
	{
		std::cout << "$" << test->getCommand() << "$" << std::endl;
		std::cout << "$" << test->getParams().back() << "$" << std::endl;
	}
	else
		std::cout << "freetext: $" << test->getFreetext() << "$" << std::endl;

	delete test;
	return (0);
}
