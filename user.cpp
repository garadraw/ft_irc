#include <iostream>
#include <algorithm>
#include "user.hpp"
#include "server.hpp"

User::User(pollfd &client, char* host, char* service, Server* server) : _client(client), _host(host), _service(service), _server(server) {
	*this->_fd = this->_client.fd;
	this->_userState = UNAUTH;
	this->_creationTime = std::time(NULL);
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

void	User::setFd(int new_fd) {
	this->_fd = &new_fd;
}

int*	User::getFd() const {
	return (this->_fd);
}

void	User::setNick(std::string nick) {
	std::transform(nick.begin(), nick.end(), nick.begin(), ::tolower); // check if tolower won't create problems later
	this->_nick = &nick;
}

std::string*	User::getNick() const {
	return (this->_nick);
}

void	User::setUsername(std::string username) {
	this->_username = &username;
}

std::string*	User::getUsername() const {
	return (this->_username);
}

void	User::setRealname(std::string realname) {
	this->_username = &realname;
}

std::string*	User::getRealname() const {
	return (this->_realname);
}

/* Sets user state to one of the given macros: UNAUTHORIZED, DISCONNECTED, CONNECTED, ... */
void	User::setState(int state) {
	this->_userState = state;
}

int	User::getState() {
	return (this->_userState);
}

bool	User::pwCheck() {
	if (this->_sentPassword == true)
		return true;
	else
		return false;
}

bool	User::isRegistered() {
	return (this->_isRegistered);
}

time_t	User::getTime() {
	return (std::time(NULL));
}

std::vector<Channel *>	User::getChannels()
{
	return (this->_channels);
}

const Channel*	User::get_channel_if_in(const std::string& channel_name)
{
	std::vector<Channel*>::iterator start = this->_channels.begin();
	std::vector<Channel*>::iterator end = this->_channels.end();

	while (start != end)
	{
		if ((*start)->getName()->compare(channel_name) == 0)
		{
			return ((*start));
		}
		start++;
	}

	return (NULL);
}

time_t	User::getCreationTime() {
	return (this->_creationTime);
}

void	User::setHost(char* host) {
	this->_host = host;
}

char**	User::getHost() {
	return (&this->_host);
}

void	User::setClient(pollfd &client) {
	this->_client = client;
}

void	User::setService(char* service) {
	this->_service = service;
}

void		Channel::deleteChannel(std::string channel)
{
		std::string str;

		std::vector<Channel*>::iterator itr;
		for (itr=begin(this->_channels); itr != end(this->_channels); ++itr) {
			if(channel == *(*itr).getName())
				_channels.erase(itr);
		}
}