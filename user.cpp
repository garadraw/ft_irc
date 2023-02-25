#include <iostream>
#include <algorithm>
#include "user.hpp"
#include "server.hpp"


		/*  Constructors and Deconstructors  */
//===================================================//


User::User(pollfd &client, char* host, char* service, Server* server) : _client(client), _host(host), _service(service), _server(server) {
	_fd = _client.fd;
	_isRegistered = false;
	_creationTime = std::time(NULL);
	std::cout << "Default user constructor called with Client Input" << std::endl;
}

User &User::operator=(const User &src) {
	this->_fd = src.getFd();
	this->_nick = src.getNick();
	return (*this);
}

User::~User() {
	std::cout << "User removed" << std::endl;
}




		/*   Getters and Setters    */
//===================================================//


int	User::getFd() const {
	return (this->_fd);
}

void	User::setFd(int new_fd) {
	this->_fd = new_fd;
}

std::string	User::getNick() const {
	return (this->_nick);
}

void	User::setNick(std::string nick) {
	std::transform(nick.begin(), nick.end(), nick.begin(), ::tolower); // check if tolower won't create problems later
	_nick = nick;
}

std::string	User::getUsername() const {
	return (this->_username);
}

void	User::setUsername(std::string username) {
	this->_username = username;
}



std::string	User::getRealname() const {
	return (this->_realname);
}

void	User::setRealname(std::string realname) {
	this->_username = realname;
}


int	User::getState() {
	return (this->_userState);
}

/* Sets user state to one of the given macros: UNAUTHORIZED, DISCONNECTED, CONNECTED, ... */
void	User::setState(int state) {
	this->_userState = state;
}


void	User::setRegistered() {
	this->_isRegistered = true;
}

bool	User::isRegistered() {
	return (this->_isRegistered);
}



std::vector<Channel *>	User::getChannels()
{
	return (this->_channelList);
}

time_t	User::getTime() {
	return (std::time(NULL));
}

time_t	User::getCreationTime() {
	return (this->_creationTime);
}

char**	User::getHost() {
	return (&this->_host);
}

void	User::setHost(char* host) {
	this->_host = host;
}


void	User::setClient(pollfd &client) {
	this->_client = client;
}

void	User::setService(char* service) {
	this->_service = service;
}


void	User::deleteChannel(std::string channel) {
		std::vector<Channel*>::iterator itr;
		for (itr=begin(this->_channelList); itr != end(this->_channelList); itr++) {
			if(channel == (*itr)->getName())
				_channelList.erase(itr);
		}
}

/* Deletes a Channel Obj Pointer from the _channelUserList */
void User::delete_Channel(Channel* channel)
{

	// Protection: Check if the _channelList is empty   Not Needed Now!!!
	/* if (this->_channelList.size() == 0)
	{
		return;
	} */

		std::vector<Channel*>::iterator itr;
		for (itr=begin(this->_channelList); itr != end(this->_channelList); itr++)
		{
			if(channel == *itr)
				_channelList.erase(itr);
		}
}




			/*    Checkers     */
//===================================================//


Channel*	User::findUserinChannel(std::string name) {
	std::vector<Channel*>::iterator itr;
	for (itr=begin(this->_channelList); itr != end(this->_channelList); itr++) {
		if (name == (*itr)->getName())
			return (*itr);
	}
	std::cerr << "Channel not found by name" << std::endl;
	return (NULL);
}


Channel*	User::get_channel_if_in(const std::string& channel_name)
{
	std::vector<Channel*>::iterator start = this->_channelList.begin();
	std::vector<Channel*>::iterator end = this->_channelList.end();

	while (start != end)
	{
		if ((*start)->getName().compare(channel_name) == 0)
		{
			return ((*start));
		}
		start++;
	}
	return (NULL);
}








// bool	User::pwCheck() {
// 	if (this->_sentPassword == true)
// 		return true;
// 	else
// 		return false;
// }