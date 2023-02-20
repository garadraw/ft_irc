#ifndef CHANNEL_HPP
 #define CHANNEL_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include <sstream>
// #include <signal.h>
#include <string>
#include <vector>
/* #include "channel.hpp" */
#include "server.hpp"

class User;

class Channel {
public:
	Channel();
	Channel(...);
	Channel&				operator=(const Channel &src);
	~Channel();

	std::vector<Channel*>	getChannels(); // gives list of all channels that exist / could als obe under User class;

	std::vector<User*>		getOperators();
	void					setOperator();

	const std::string&		getName();
	size_t					getMaxClients() const;
	size_t					getClientCount() const;
	void					addUser(User* user);
	void					notify_others(const std::string& msg, User* skip);

	std::string				channelName;

private:
	std::vector<User*> 		_operatorList;
	std::string				_creatorUser;
	std::vector<User*> 		_userLogList; // all users joined the channel

	size_t max_clients;
	size_t count_clients;
};

#endif