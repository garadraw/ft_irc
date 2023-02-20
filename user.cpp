#include <iostream>
#include <algorithm>
#include "user.hpp"
#include "server.hpp"

User::User(pollfd &client, char* host, char* service) : _client(client), _host(host), _service(service) {
	this->_fd = this->_client.fd;
	this->_userState = UNAUTHORIZED;
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
	this->_fd = new_fd;
}

int		User::getFd() const {
	return (this->_fd);
}

void	User::setNick(std::string nick) {
	std::transform(nick.begin(), nick.end(), nick.begin(), ::tolower); // check if tolower won't create problems later
	this->_nick = nick;
}

std::string	User::getNick() const {
	return (this->_nick);
}

void	User::setUsername(std::string username) {
	this->_username = username;
}

std::string	User::getUsername() const {
	return (this->_username);
}

void	User::setRealname(std::string realname) {
	this->_username = realname;
}

std::string	User::getRealname() const {
	return (this->_realname);
}

/* Sets user state to one of the given macros: UNAUTHORIZED, DISCONNECTED, CONNECTED, ... */
void	User::setState(int state) {
	// ...
}

int	User::getState() {
	return (this->_userState);
}

bool	User::isRegistered() {
	return (this->_isRegistered);
}

time_t	User::getTime() {
	return (std::time(NULL));
}
