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
/* #include "channel.hpp" */

class Channel;
class User;

class Server {
	public:
		Server();
		~Server();
		Server(std::string pass, int port);

		void		createServer();
		void		initClient();
		void		pollLoop();
		void		pingClient();

		void					setPass(std::string pass);
		std::string				getPass() const;

		int						fd_server;
		struct pollfd 			clients[1024];

		// Channel*				getChannel(const std::string& channel_name);

	private:
		void					readInput(int client_no);
		void					acceptCall();

		int						_port;
		std::string				_pass;
		std::string				_serverName;
		std::vector<User*> 		_userList;
		std::vector<Channel*> 	_channelList;

		std::vector<std::string>	parseIncomingMsg(std::string buffer);
};

#endif