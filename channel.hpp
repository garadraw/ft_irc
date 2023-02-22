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
	Channel(std::string newName);
	Channel&				operator=(const Channel &src);
	~Channel();

	void					addCreator(User* creator); // adds the creator of channel

	std::vector<User*>		getOperators(); // returns list of operators
	void					setOperator(User* newOp); // adds User to operator list

	void					addBanned(User* toBan); // adds user to Banned list
	bool					ifBanned(User* ifBan); // checks if user is banned from channel;
	void					deleteUser(std::string _nick);

	const std::string*		getName();
	size_t					getMaxClients() const;
	size_t					getClientCount() const;
	void					addUser(User* user);
	void					delete_user(User* user);
	std::string				findAllUsers();
	int						ifOperator();
	// void					notify_others(const std::string& msg, User* skip);

	// std::string*			channelName;





private:
	std::string				_channelName;
	// size_t const			_maxClients;
	// size_t				_countClients;

	User*					_creatorUser; // user who created channel will always be operator
	std::vector<User*> 		_operatorList; // all operators on channel
	std::vector<User*> 		_userLogList; // all users joined the channel
	std::vector<User*>		_bannedList; // all banned users

	User*					_admin;
};

#endif