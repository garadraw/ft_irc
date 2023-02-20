#ifndef USER_HPP
 #define USER_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include <sstream>
#include <ctime>
#include <time.h>
// #include <signal.h>
#include <string>
#include <vector>

#include "channel.hpp"
#include "server.hpp"

#define UNAUTHORIZED 1
#define CONNECTED 2
#define DISCONNECTED 3

class Channel;

class User {
public:
	User(pollfd &client, char *host, char *service);
	User&					operator=(const User &src);
	~User();

	void					setFd(int new_fd);
	int 					getFd() const;

	void 					setNick(std::string nick); // set in lowercase / convert
	std::string				getNick() const;

	void					setUsername(std::string username);
	std::string				getUsername() const;
	
	void					setRealname(std::string realname);
	std::string				getRealname() const;

	void					setState(int state);
	int						getState(); 

	bool					isRegistered();

	time_t					getTime(); // returns actual time of call (used for Ping)

	//void					joinChannel(); // makes user join a channel; prompts server to send message to all relevant users about joining; 
	//void					leaveChannel(); // makes user itself leave channel; sends message that user left channel;
	//void					createChannel(); // ourUser.createChannel() _ creates new channel, and user is made operator; called inside joinChannel();
	//void					removeChannel(); // if last user disconnects, channel is removed?;


	// All following functions are in userUtils.cpp file:
	//std::string 			getPrefix() const;
	//void					reply(const std::string& reply_msg);
	//void					execute_join_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
	//void 					write_msg(const std::string& msg) const;


	std::vector<Channel *>	getChannels();

	bool					isOperator(); // server operator: channel operators are stored in channel object
	void					setOperator();

private:
	std::string				_nick;
	int						_fd;
	char*					_host;
	char*					_service;
	int						_userState;
	bool					_isRegistered;
	time_t					_creationTime;
	std::string				_username;
	std::string				_realname;
	std::vector<Channel *>	_channels; // list of channels the user is part of for cross-exchanging data
	struct pollfd &			_client; 
};

#endif