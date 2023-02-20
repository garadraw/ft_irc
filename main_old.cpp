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
#include <vector>

#include "user.hpp"


int fd_global;
void	siginthandler(int signum)
{
close(fd_global);
exit(1);
}

/* Create a socket */
/*	-difference AF(adress family) and PF(protocol family) PF was intended to
	handle several protocols this never happened so AF is what you should use
	-difference INET(v4) and INET6(v6) is the size of their address space
	 INET6 improved features:
	 -better routing and security
	 -eliminates the need for address translation, which is necessary in INET
	-SOCK_STREAM type provides sequenced, reliable, two-way connection based byte streams
	 SOCK_DGRAM socket supports datagrams (connectionless, unreliable messages of a fixed
	 (typically small) maximum length)
	-SOCK_RAW sockets provide access to internal network protocols and interfaces (only super-user)*/
int createserver(void)
{
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	//? check what the sockets are
	if (listening == -1)
	{
		std::cerr << "Can't create a socket!" << std::endl;
		return (-1);
	}
	// TODO add setsockopt function and research
	/* Bind the socket to a IP / Port */
	/* INADDR_ANY is a macro to get the IP address of the host machine (server)
	   you could also use inet_addr("127.0.0.1") to assign it manually */
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_addr.s_addr = htonl(INADDR_ANY);
	// inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	hint.sin_port = htons(55000); // here we should do the byteorder network/host using htons and htonl
	if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Can't bind to IP/Port!" << std::endl;
		return (-2);
	}
	// Mark the socket for listening in
	if (listen(listening, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen!" << std::endl;
		return (-3);
	}
	return (listening);
}

void readinput(int clientfd, pollfd *clients, vector<User> *userList)
{
	// While receiving display message, echo message
	char buf[4096];
	int i = 0;
	// while (i <= 10)
	// {
	// Clear the buffer
	memset(buf, 0, 4096);
	// Wait for a message
	int bytesRecv = recv(clientfd, buf, 4096, 0);
	if (bytesRecv == -1)
	{
		std::cerr << "There was a connection issue!" << std::endl;
	}
	else if (bytesRecv == 0)
	{
		if (close(clientfd) == -1)
			std::cerr << "close!" << std::endl;
		std::cerr << "The Client disconnected!" << std::endl;
		clientfd *= -1; // it will be ignored in future
	}
	else // data from client
	{
		// Display message
		std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
	}


/* 	PSEUDOCODE PARSER: 
		1. STRING is in BUF;
		2. Send BUF to initParser(std::string* and clientlist)
		3. ... finds out which commands have been requested; */

	// Send message
	// std::string nick = "mjpro";
	std::string user = "mj_nick";
	std::string channel = "#ch1";
	std::string message = buf;

	std::ostringstream cmd;
	cmd //<< "NICK " << nick << "\r\n"
		<< "USER " << user << "\r\n"
	// 	// << "JOIN " << channel << "\r\n"
		<< "PRIVMSG " << channel << " :" << message << "\r\n";
	std::string cmd_str = cmd.str();
	for (int i = 1; i < 1024; i++)
	{
		if (clients[i].fd != -1 && clients[i].fd != clientfd)
		{
			send(clients[i].fd, cmd_str.c_str(), cmd_str.size(), 0);
			// std::cout << "Message: " << buf << std::endl;
		}
	}
	// std::cout << "Message is as:" << message << "$\n";
	// send(clientfd, buf, bytesRecv + 1, 0);
	// }
}

/* Accepting a call */
void acceptcall(int server, pollfd *client, vector<User> *userList)
{
	for (int i = 0; i < 1024; i++)
	{
		if ((client[i].revents & POLLIN) == POLLIN) // fd is ready for reading
		{
			if (client[i].fd == server) // request for new connection
			{

				std::cout << "New Connection " << std::endl;
				sockaddr_in user;
				socklen_t userSize = sizeof(user);
				char host[NI_MAXHOST];
				char service[NI_MAXSERV];
				memset(host, 0, NI_MAXHOST);
				memset(service, 0, NI_MAXSERV);
				int userSocket = accept(server, (sockaddr *)&user, &userSize);
				if (userSocket == -1)
				{
					std::cerr << "Problem with client connecting!" << std::endl;
					return;
				}

				// TODO the next 10 lines are not needed, just there for testing purpose
				int result = getnameinfo((sockaddr *)&user, userSize, host, NI_MAXHOST, service, NI_MAXSERV, 0);
				if (result)
				{
					std::cout << host << " connected on " << service << std::endl;
				}
				else
				{
					inet_ntop(AF_INET, &user.sin_addr, host, NI_MAXHOST);
					std::cout << host << " connected on " << ntohs(user.sin_port) << std::endl;
				}
				int j = 0;
				for (; j < 1024; j++)
					if (client[j].fd == -1)
						break;
				if (j < 1024)
				{
					client[j].fd = userSocket;
					client[j].events = POLLIN; 
					client[j].revents = 0;
					/* if (user already there -> iterate through userlist)
						then assign fd to the user;
					else */
					User newUser(client[j].fd);
					userList.push_back(newUser);
				}
				else
				{
					close(userSocket);
				}
			}
			else // data from an existing connection, recieve it
			{
				readinput(client[i].fd, client, userList);
			}
		}
	}
}
int main()
{
	signal(SIGINT, siginthandler);
	signal(SIGQUIT, siginthandler);
	struct pollfd clients[1024];
	std::vector<User> userList;
	std::vector<Channel> channelList;
	int server = createserver();
	fd_global = server;
	for (int i = 0; i < 1024; i++)
	{
		clients[i].fd = -1;
		clients[i].events = 0;
		clients[i].revents = 0;
	}
	clients[0].fd = server;
	clients[0].events = POLLIN;
	// TODO create mainloop here
	while (1)
	{
		switch (poll(clients, 1024, 10000))
		{
		case 0:
			std::cout << "Should not be possible" << std::endl;
			break;
		case -1:
			std::cout << "could not be possible" << std::endl;
			break;
		default:
			// std::cout << "begin of the default switch" << std::endl;
			acceptcall(server, clients, &userList);
			// readinput(clients);
			break;
		// if (something_time) -> ft_checkUserList(&userList); // every 30 secs the userlist is checked: it iterates through the list and checks if all data for connections is there;
		}
	}

	// Close the listening socket
	close(server); //! should we close it ?
	return (0);
}
