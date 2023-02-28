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
#include <cstdlib>

#include "server.hpp"

void	signalHandler(int sig) {
	std::cout << "Attempting Quit Server" << std::endl;
	if (sig == SIGINT)
	{
		Server::_active = false;
		signal(SIGINT, SIG_IGN);
	}
	std::cout << "Quit Server" << std::endl;
}

int	main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Please enter the right amount of arguments" << std::endl;
		return (0);
	}
	int	port = atoi(argv[1]);
	std::string serverPass(argv[2]);

	Server ourServer(serverPass, port);
	ourServer.createServer();
	signal(SIGINT, signalHandler);
	ourServer.initClient();
	ourServer.clients[0].fd = ourServer.fd_server;
	ourServer.clients[0].events = POLLIN;
	ourServer.pollLoop();

	close(ourServer.fd_server);
	return (0);
}
