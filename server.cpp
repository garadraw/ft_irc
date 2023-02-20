#include <errno.h>
#include "server.hpp"


Server::Server(std::string serverPass, int port): _pass(serverPass), _port(port)
{
	std::cout << "Server Object Created" << std::endl;
}

Server::~Server()
{
	std::cout << "Deconstructor Called" << std::endl;
}

void	Server::setPass(std::string pass) {
	this->_pass = pass;
}

std::string	Server::getPass() const {
	return (this->_pass);
}

void Server::createServer(void)
{
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		std::cerr << "Can't create a socket!" << std::endl;
		/* return (-1); */ // as of now the f.return type is void, we removed this; has to be resculptured;
	}
	// TODO add setsockopt function and research
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_addr.s_addr = htonl(INADDR_ANY);
	// inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	hint.sin_port = htons(this->_port);
	if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Can't bind to IP/Port!" << std::endl;
		std::cerr << errno << std::endl;
		/* return (-2); */
	}
	if (listen(listening, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen!" << std::endl;
		/* return (-3); */
	}
	this->fd_server = listening;
}

//comented out to compile and work on authentification
void Server::readInput(int client_no)
{
	char buf[4096];
	int i = 0;
	memset(buf, 0, 4096);
	int bytesRecv = recv(this->clients[client_no].fd, buf, 4096, 0);
	if (bytesRecv == -1)
	{
		std::cerr << "There was a connection issue!" << std::endl;
	}
	else if (bytesRecv == 0)
	{
		if (close(this->clients[client_no].fd) == -1)
			std::cerr << "close!" << std::endl;
		std::cerr << "The Client disconnected!" << std::endl;
		this->clients[client_no].fd *= -1; // it will be ignored in future
	}
	else // data from client
	{
		// Display message
		std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
	}
	//parsing buffer to vector of two string elements
		//first element is prefix or empty
		//second element is always Command and Parameters to it

	//std::vector<std::string> bufferParsed = parseIncomingMsg(std::string(buf, 0, bytesRecv));

	// WORK WITH BUFFER AFTER PARSING

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
		if (clients[i].fd != -1)
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
void Server::acceptCall()
{
	for (int i = 0; i < 1024; i++)
	{
		if ((this->clients[i].revents & POLLIN) == POLLIN) // fd is ready fo reading
		{
			std::cout << "this client i fd is: " << this->clients[i].fd << "\n and this fd_server is: " << this->fd_server << std::endl;
			if (this->clients[i].fd == this->fd_server) // request for new connection
			{

				std::cout << "New Connection " << std::endl;
				sockaddr_in user;
				socklen_t userSize = sizeof(user);
				char host[NI_MAXHOST];
				char service[NI_MAXSERV];
				memset(host, 0, NI_MAXHOST);
				memset(service, 0, NI_MAXSERV);
				int userSocket = accept(this->fd_server, (sockaddr *)&user, &userSize);
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
				// untill here
				
				int j = 0;
				for (; j < 1024; j++)
					if (clients[j].fd == -1)
						break;
				if (j < 1024)
				{
					clients[j].fd = userSocket;
					clients[j].events = POLLIN; //? do we need this line
					clients[j].revents = 0;

					/* if (..)
					else */ 
						User	*newUser = new User(clients[j], host, service);
						this->_userList.push_back(newUser);
					
					// Testing ---
					std::ostringstream cmd;
					// cmd << "%C29*%O$tCapabilities acknowledged: %C29$2%O";
					cmd << "%UChannel          mjpro   fun";
					std::string cmd_str = cmd.str();
					send(clients[j].fd, cmd_str.c_str(), cmd_str.size(), 0);
					// --- until here

				}
				else
				{
					std::cout << "Server Capacity for Clients is Full" << std::endl;
					close(userSocket);
				}
			}
			else // data from an existing connection, recieve it
			{
				this->readInput(i);
			}
		}
	}
}

void	Server::initClient()
{
	for (int i = 0; i < 1024; i++)
	{
		this->clients[i].fd = -1;
		this->clients[i].events = 0;
		this->clients[i].revents = 0;
	}
}

void	Server::pollLoop()
{
	while (1)
	{
		switch (poll(this->clients, 1024, 10000))
		{
		case 0:
			std::cout << "Should not be possible" << std::endl;
			break;
		case -1:
			std::cout << "could not be possible" << std::endl;
			break;
		default:
			this->acceptCall();
			// pingClient();
			// readinput(clients);
			break;
		}
	}
}

/* Channel* Server::getChannel(const std::string& channel_name)
{
	std::vector<Channel*>::iterator start = this->_channelList.begin();
	std::vector<Channel*>::iterator end = this->_channelList.end();

	while (start != end)
	{
		if (channel_name.compare((*start)->getName()) == 0)
		{
			return ((*start));
		}
		start++;
	}

	std::cout << "no channel found: returning NULL*" << std::endl;
	return (NULL);
} */

