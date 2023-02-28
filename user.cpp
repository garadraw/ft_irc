#include <iostream>
#include <algorithm>
#include "user.hpp"
#include "server.hpp"


		/*  Constructors and Deconstructors  */
//===================================================//


User::User(pollfd &client, char* host, char* service, Server* server) : replyMessage(), _server(server), _channelList(), _nick(""), _fd(), _host(host), _service(service), _isRegistered(false), _username(), _realname(), _client(client) {
	_fd = _client.fd;
	/* _username = "";
	_realname = "IRC User";
	_isRegistered = false;
	_creationTime = std::time(NULL);
	_channelList = std::vector<Channel*>(); */
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
	std::transform(nick.begin(), nick.end(), nick.begin(), ::tolower);
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

/* time_t	User::getTime() {
	return (std::time(NULL));
} */
/* 
time_t	User::getCreationTime() {
	return (this->_creationTime);
} */

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

	// Protection: Check if the _channelList is empty
	if (this->_channelList.size() == 0)
	{
		return ;
	}
	std::vector<Channel*>::iterator itr;
	for (itr=this->_channelList.begin(); itr != this->_channelList.end(); itr++)
	{
		if(channel == (*itr)->getName())
		{
			_channelList.erase(itr);
			break ;
		}
	}
}

/* Deletes a Channel Obj Pointer from the _channelUserList */
void User::delete_Channel(Channel* channel)
{
	// Protection: Check if the _channelList is empty
	if (this->_channelList.size() == 0)
	{
		return ;
	}
	std::vector<Channel*>::iterator itr;
	for (itr=this->_channelList.begin(); itr != this->_channelList.end(); itr++)
	{
		if(channel == *itr) {
			std::cout << "Reached in delete_Channel" << std::endl;
			
			_channelList.erase(itr);
			break ;
		}
	}
}


			/*    Checkers     */
//===================================================//


Channel*	User::findUserinChannel(std::string name)
{
	// Protection: Check if the _channelList is empty
	if (this->_channelList.size() == 0)
	{
		return (NULL);
	}
	std::vector<Channel*>::iterator itr;
	for (itr=this->_channelList.begin(); itr != this->_channelList.end(); itr++) {
		if (name == (*itr)->getName())
			return (*itr);
	}
	std::cerr << "Channel not found by name" << std::endl;
	return (NULL);
}


Channel*	User::get_channel_if_in(const std::string& channel_name)
{
	std::cout << "DEBUG: Inside get_channel_if_in() nr 1" << std::endl;
	if (!this->_channelList.empty()) {
		std::cout << "DEBUG: Inside get_channel_if_in() nr 2" << std::endl;
		std::vector<Channel*>::iterator itro;
		for (itro = this->_channelList.begin(); itro != this->_channelList.end(); itro++)
		{
			std::cout << "DEBUG: Inside get_channel_if_in() nr 3; this->_channelList.size() is " << this->_channelList.size() << std::endl;
			if (channel_name == (*itro)->getName())
			{
				std::cout << "DEBUG: Inside get_channel_if_in() nr 4, before the return" << std::endl;
				return ((*itro));
			}
		}
	}
	return (NULL);
}