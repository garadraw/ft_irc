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

// int fd_global;

/* void siginthandler(int signum)
{
	close(ourSever.ft_server);
	exit(1);
} */

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Please enter the right amount of arguments" << std::endl;
	}
	int	port = atoi(argv[1]);
	std::string serverPass(argv[2]);
/* 	std::string serverName(argv[3]); */

	Server ourServer(serverPass, port);
    ourServer.createServer();
	// fd_global = ourServer.fd_server;
	/* signal(SIGINT, siginthandler);
	signal(SIGQUIT, siginthandler); */
	ourServer.initClient();
	ourServer.clients[0].fd = ourServer.fd_server;
	ourServer.clients[0].events = POLLIN;
	ourServer.pollLoop();

	close(ourServer.fd_server);
	return (0);
}
