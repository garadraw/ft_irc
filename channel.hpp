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

	const std::string		getName();
	size_t					getMaxClients() const;
	size_t					getClientCount() const;

	void					addCreator(User* creator); // adds the creator of channel

	std::vector<User*>		getOperators(); // returns list of operators
	void					setOperator(User* newOp); // adds User to operator list


	std::vector<User*>		getUsers(); // returns a list of normal users

	void					addBanned(User* toBan); // adds user to Banned list
	bool					ifBanned(User* ifBan); // checks if user is banned from channel;
	void					deleteUser(std::string _nick);
	void					deleteChannel(std::string channel);


	void					addUser(User* user);
	void					delete_user(User* user);



	std::string				findAllUsers(); // returns a string of all users, OPERATORS will have a @ infront

	int						ifOperator(std::string _nick);
	// bool					ifIsOperator(User* checkUser); // like ifOperator, but accepts a User* as argument, and returns boolean

	int						ifJoined(std::string _nick);
	User*					isUserinChannelbys(std::string _nick); // Finds if Nick in Channel and returns a Pointer to User Obj
	User*					isUserinChannelbyp(User* user); // Finds if User Obj Pointer in Channel and returns a Pointer to User Obj
	int						is_User_Operator(User* user); // Finds if User Obj Pointer in OperatorList


	// User *					get_user_if_in(std::string& _nick);
	void					notify_others(std::string cmd_name, const std::string& msg, User* skip);

	size_t					_countClients;

private:
	std::string				_channelName;
	size_t const			_maxClients;

	User*					_creatorUser; // user who created channel will always be operator
	std::vector<User*> 		_operatorList; // all operators on channel
	std::vector<User*> 		_userLogList; // all users joined the channel
	std::vector<User*>		_bannedList; // all banned users

	User*					_admin;
};

#endif