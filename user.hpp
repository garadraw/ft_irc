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
// #include <signal.h>
#include <string>
#include <vector>

#include "channel.hpp"
#include "server.hpp"

#define UNAUTH 1
#define CONNECTED 2
#define DISCONNECTED 3

class Channel;
class Server;

class User {
public:
	User(pollfd &client, char* host, char* service, Server* server);

	User&					operator=(const User &src);
	~User();

	void					setFd(int new_fd);
	int* 					getFd() const;

	void 					setNick(std::string nick); // set in lowercase / convert
	std::string*			getNick() const;

	void					setUsername(std::string username);
	std::string*			getUsername() const;
	
	void					setRealname(std::string realname);
	std::string*			getRealname() const;

	void					setState(int state);
	int						getState(); 

	bool					pwCheck();

	bool					isRegistered();

	time_t					getTime(); // returns actual time of call (used for Ping)
	time_t					getCreationTime(); // returns time of creation of user

	void					setHost(char* host);
	char**					getHost();

	void					setClient(pollfd &client); // stored client info on user profile
	void					setService(char* service);

	//void					joinChannel(); // makes user join a channel; prompts server to send message to all relevant users about joining; 
	//void					leaveChannel(); // makes user itself leave channel; sends message that user left channel;
	//void					createChannel(); // ourUser.createChannel() _ creates new channel, and user is made operator; called inside joinChannel();
	//void					removeChannel(); // if last user disconnects, channel is removed?;


	// All following functions are in userUtils.cpp file:
	//std::string 			getPrefix() const;
	void					reply(const std::string& reply_msg);
	void					execute_join_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
	//void 					write_msg(const std::string& msg) const;
	void					execute_kick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
	void 					execute_invite_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
	void 					execute_quit_cmd(const std::string& cmd_name, std::vector<std::string> args);
	void					execute_ping_cmd(const std::string& cmd_name, std::vector<std::string> args);
	void					execute_nick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
	void					execute_user_cmd();

	std::vector<Channel *>	getChannels();
	const Channel*			get_channel_if_in(const std::string& channel_name);

	bool					isOperator(); // server operator: channel operators are stored in channel object
	void					setOperator();
	void					deleteChannel(std::string _nick);




private:
	std::string*			_nick;
	int*					_fd;
	char*					_host;
	char*					_service;
	int						_userState;
	bool					_isRegistered;
	bool					_sentPassword;
	time_t					_creationTime;
	std::string*			_username;
	std::string*			_realname;
	std::vector<Channel *>	_channels; // list of channels the user is part of for cross-exchanging data
	struct pollfd &			_client;
	Server*					_server;
};

#endif