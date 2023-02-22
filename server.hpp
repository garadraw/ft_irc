#ifndef SERVER_HPP
 #define SERVER_HPP

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
// #include <signal.h>
#include <vector>

#include "user.hpp"
#include "responses.hpp"
/* #include "channel.hpp" */

class Channel;
class User;

class Server {
	public:
		Server();
		~Server();
		Server(std::string pass, int port);

		int			createServer();
		void		initClient();
		void		pollLoop();
		void		pingClient();

		void					setPass(std::string pass);
		std::string				getPass() const;

		int						fd_server;
		struct pollfd 			clients[1024];

		void					killUser(User * user);
		Channel*				findChannel(std::string name); // finds a channel by name

		// Channel*				getChannel(const std::string& channel_name);

	private:
		int						isUserInServer(char* host); // checks if user has already been registered before
		void					reconnectUser(pollfd &client, char* host, char* service); // reconnects user (by host) to existing user profile

		User*					findByFd(int clientFd); // finds user by FD
		User*					findByNick(std::string nick); // finds user by nickname
		// User*					findByHost()
		bool					authUser(User* activeUser); // checks if user is authed, meaning: pass, nick and username provided
		void					kickUser(User* toBeKicked);
		int						readInput(int client_no);
		void					acceptCall();

		int						_port;
		std::string				_pass;
		std::string				_serverName;
		std::vector<User*> 		_userList;
		std::vector<Channel*> 	_channelList;

		std::vector<std::string>	parseIncomingMsg(std::string buffer);
};

#endif