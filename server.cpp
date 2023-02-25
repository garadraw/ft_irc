#include <errno.h>
#include <ctime>
#include "server.hpp"
#include "Message.hpp"


		/*  Constructors and Deconstructors  */
//===================================================//


Server::Server(std::string serverPass, int port): _pass(serverPass), _port(port)
{
	_serverName = "OurServer";
	std::cout << "Server Object Created" << std::endl;
}

Server::~Server()
{
	std::cout << "Deconstructor Called" << std::endl;
}



		/*   Getters and Setters    */
//===================================================//


void	Server::setPass(std::string pass) {
	this->_pass = pass;
}

std::string	Server::getPass() const {
	return (this->_pass);
}

void	Server::addChannel(Channel * newChannel)
{
	this->_channelList.push_back(newChannel);
}



			/*    Checkers     */
//===================================================//

User*	Server::findByFd(int clientFd) {
	std::vector<User*>::iterator itr;
	for (itr=begin(this->_userList); itr != end(this->_userList); itr++) {
		if (clientFd == (*itr)->getFd())
			return (*itr);
	}
	std::cerr << "User FD not found" << std::endl;
	return (NULL);
}

User*	Server::findByNick(std::string nick) {
	std::vector<User*>::iterator itr;
	for (itr=begin(this->_userList); itr != end(this->_userList); itr++) {
		if (nick == (*itr)->getNick())
			return (*itr);
	}
	std::cerr << "User nickname not found" << std::endl;
	return (NULL);
}

Channel*	Server::findChannel(std::string name) {
	std::vector<Channel*>::iterator itr;
	for (itr=begin(this->_channelList); itr != end(this->_channelList); itr++) {
		if (name == (*itr)->getName())
			return (*itr);
	}
	std::cerr << "Channel not found by name" << std::endl;
	return (NULL);
}

/* Check if the User is already in the Server Object's User List */
int	Server::isUserInServer(char* host) {
	for (std::vector<User *>::iterator it = this->_userList.begin(); it != this->_userList.end(); ++it)
	{
		if ((*(*it)).getHost() == &host)
			return (1);
	}
	std::cerr << "User not Found in Server User List" << std::endl;
	return (0);
}





			/*   Server Actions  */
//==============================================//


void	Server::kickUser(User* toBeKicked) {
	//close fd? 
	// delete toBeKicked;
}

void	Server::killUser(User * user)
{
	std::vector<Channel*>::iterator start = user->getChannels().begin();
	std::vector<Channel*>::iterator end = user->getChannels().end();
	if (user->getChannels().size() > 0)
	{	
		while (start != end)
		{
			std::vector<Channel*> channels = user->getChannels();
			channels[0]->delete_user(user);
			user->getChannels().erase(start);
			start++;
		}
	}
	std::vector<User*>::iterator start_U = _userList.begin();
	std::vector<User*>::iterator end_U = _userList.end();
	if (_userList.size() > 0)
	{
		while (start_U != end_U)
		{
			if (*start_U == user)
			{
				_userList.erase(start_U);
				break ;
			}
			start_U++;
		}
	}
	delete user;
}


			/*    Socket Actions    */
//===================================================//


void	Server::reconnectUser(pollfd &client, char* host, char* service) {
	User* foundUser;
	for (std::vector<User *>::iterator it = this->_userList.begin(); it != this->_userList.end(); ++it)
	{
		if ((*(*it)).getHost() == &host)
			*foundUser = *(*it);
			foundUser->setClient(client);
			foundUser->setHost(host);
			foundUser->setService(service);
	}
}


// Disable functionality, BUT usable! Not required by Subject PDF.
/* bool	Server::authUser(User* activeUser) {
	if (activeUser->pwCheck() == false)
		return false;
	if (!activeUser->getNick().c_str())
		return false;
	if (!activeUser->getUsername().c_str())
		return false;
	else
		return true;
} */

/* void	Server::kickUser(User* toBeKicked) {
	//close fd? 
	// delete toBeKicked;
} */





int Server::createServer(void)
{
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		std::cerr << "Can't create a socket!" << std::endl;
		return (-1);
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
		return (-2);
	}
	if (listen(listening, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen!" << std::endl;
		return (-3);
	}
	this->fd_server = listening;
	return (1);
}

/* Reads input from an fd, returns message if received successfully. */
int Server::readInput(int client_no)
{
	char buf[4096];
	int i = 0;
	memset(buf, 0, 4096);
	int bytesRecv = recv(this->clients[client_no].fd, buf, 4096, 0);
	if (bytesRecv == -1)
	{
		std::cerr << "There was a connection issue!" << std::endl;
		return (-1);
	}
	else if (bytesRecv == 0)
	{
		if (close(this->clients[client_no].fd) == -1)
			std::cerr << "close!" << std::endl;
		std::cerr << "The Client disconnected!" << std::endl;
		this->clients[client_no].fd *= -1; // it will be ignored in future
		return (-1);
	}
	else // data received from client
	{
		// Display message
		std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
	}
	
	//std::vector<std::string> bufferParsed = parseIncomingMsg(std::string(buf, 0, bytesRecv));
	this->msg = new Message(std::string(buf, 0, bytesRecv));
	this->msg->message = buf;
	// std::cout << "Reached in readInput" << std::endl;

	return (1);
}

/* Created for testing, can be used to send messages to any user. Input: pointer to user. */
void Server::sendmsg(User* foundUser)
{
	for (int i = 1; i < 1024; i++)
	{
		if (clients[i].fd != -1)
		{
			send(foundUser->getFd(), this->msg->message.c_str(), this->msg->message.size(), 0);
		}
	}
}

/* Accepting a call */
int Server::acceptCall()
{
	time_t	timeNow = time(NULL);
	for (int i = 0; i < 1024; i++)
	{
		if ((this->clients[i].revents & POLLIN) == POLLIN) // fd is ready for reading
		{
			std::cout << "Message incoming from fd: " << this->clients[i].fd << std::endl;
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
					return (-1);
				}

				// Next 10 lines deal with client info:
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
				// until here
				
				int j = 0;
				for (; j < 1024; j++)
					if (clients[j].fd == -1)
						break;
				if (j < 1024)
				{
					clients[j].fd = userSocket;
					clients[j].events = POLLIN;
					clients[j].revents = 0;

					// Following lines are disable for evals. Reason: this check is not needed for our version of IRC.
						// WHat does the check do? It only lets each computer join the server once, and reconnects the
						// user in case it has logged in from the same machine before.
					//if (this->isUserInServer(host) == 1)
					//	this->reconnectUser(clients[j], host, service);
					//else {
						std::cout << "New connection data:\n fd_server: " << this->fd_server << std::endl;
						std::cout << "handler: " << this->handler << std::endl;
						std::cout << "msg: " << this->msg << std::endl;
						std::cout << "pass: " << this->_pass << std::endl;
						std::cout << "port: " << this->_port << std::endl;
						std::cout << "host: " << host << std::endl;
						std::cout << "service: " << service << std::endl;
						// std::cout << "channelList: " << this->_channelList[0] << std::endl;
						// std::cout << "userList: " << this->_userList[0] << std::endl;
						// std::cout << "channelList: " << this->_channelList[0] << std::endl;
						std::cout << "*********************************************************************************" << std::endl;
						User	*newUser = new User(clients[j], host, service, this);
						std::cout << "Reached debugging point 1" << std::endl;
						this->_userList.push_back(newUser);
					//}
					
							// Testing ---
									/* std::ostringstream cmd;
											// cmd << "%C29*%O$tCapabilities acknowledged: %C29$2%O";
									cmd << "%UChannel          mjpro   fun";
									std::string cmd_str = cmd.str();
									send(clients[j].fd, cmd_str.c_str(), cmd_str.size(), 0); */
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
				User* foundUser = this->findByFd(this->clients[i].fd);
				// authUser disables for now - but can be enabled later. 
				// What does it do? Checks if the user is authenticated. It is NOT NEEDED for evals, because
				// our chosen HexChat clients only lets users log in if they authenticate.
				/* if (foundUser != 0) {
					if (this->authUser(foundUser) == false && (timeNow - foundUser->getCreationTime() >= 6000))
					{
						// add function here to kick user from server: kickUser()
						return (-1);
					}
				}
				else
					return (1); */
				// std::cout << "Reached debugging point 0" << std::endl;
				this->readInput(i);
				handler->start(foundUser, msg->message);
				// std::cout << "Reached debugging point 2" << std::endl;
				// sendmsg(foundUser);
			}
		}
	}
	return (1);
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
			// std::cout << "Should not be possible" << std::endl;
			break;
		case -1:
			// std::cout << "could not be possible" << std::endl;
			break;
		default:
			this->acceptCall();
			// pingClient();
			// readinput(clients);
			break;
		}
	}
}


// bool	Server::authUser(User* activeUser) {
// 	if (activeUser->pwCheck() == false)
// 		return false;
// 	if (!activeUser->getNick().c_str())
// 		return false;
// 	if (!activeUser->getUsername().c_str())
// 		return false;
// 	else
// 		return true;
// }


