#include "channel.hpp"

Channel::Channel(std::string newName) : _channelName(newName), _maxClients(60) {
	std::cout << "New channel created with name " << _channelName << std::endl;
}

Channel&	Channel::operator=(const Channel &src) {
	this->channelName = src.channelName;
	this->_bannedList = src._bannedList;
	this->_operatorList = src._operatorList;
	return (*this);
}

Channel::~Channel() {
	std::cout << "Channel destroyed" << std::endl;
}


void	Channel::addCreator(User* creator) {
	this->_creatorUser = creator;
	this->setOperator(creator);
}

std::vector<User*>	Channel::getOperators() {
	return (this->_operatorList);
}

void	Channel::setOperator(User* newOp) {
	this->_operatorList.push_back(newOp);
}


void	Channel::addBanned(User* toBan) {
	this->_bannedList.push_back(toBan);
}

void Channel::delete_user(User* user)
{
	std::vector<User*>::iterator start = this->_userLogList.begin();
	std::vector<User*>::iterator end = this->_userLogList.end();

	if (this->_userLogList.size() == 0)
	{
		return;
	}

	while (start != end)
	{
		if ((*(*start)).getNick()->compare(*user->getNick()) == 0)
		{
			this->_userLogList.erase(start);
		}
		start++;
	}

	if (this->_admin == user)
	{
		_admin = *(this->_userLogList.begin());
		std::cout << _admin->getNick() << " is now admin of channel: " << this->getName() << std::endl;
	}
}

bool	Channel::ifBanned(User* ifBan) {
	std::vector<User*>::iterator itr;
	for (itr=begin(this->_bannedList); itr != end(this->_bannedList); ++itr) {
		if ((char *)ifBan->getHost() == *(*itr)->getHost())
			return true;
	}
	std::cerr << "User not found as banned" << std::endl;
	return false;
}


const std::string* Channel::getName()
{
	return (&this->_channelName);
}


size_t Channel::getMaxClients() const
{
	return (this->_maxClients);
}

size_t	Channel::getClientCount() const
{
	return (this->_countClients);
}

void	Channel::addUser(User* user)
{
	this->_userLogList.push_back(user);
}

/* void Channel::notify_others(const std::string& msg, User* skip)
{
	std::vector<User*>::iterator start = this->_userLogList.begin();
	std::vector<User*>::iterator end = this->_userLogList.end();

	while (start != end)
	{
		if ((*start)->getNick().compare(skip->getNick) == 0)
		{
			continue;
		}
		(*start)->write_msg(msg);
		start++;
	}
} */

std::string		Channel::findAllUsers()
{
		std::string str;

		std::vector<Channel*>::iterator itr;
		for (itr=begin(this->_userLogList); itr != end(this->_userLogList); ++itr) {
			str += *(*itr)._nick + " ";
			return (str);
}

int		Channel::ifOperator(std::string _nick)
{
		std::string str;

		std::vector<User*>::iterator itr;
		for (itr=begin(this->_operatorList); itr != end(this->_operatorList); ++itr) {
			if(_nick == *(*itr).getNick())
				return(1);
		return (0);
}

void		Channel::deleteUser(std::string _nick)
{
		std::string str;

		std::vector<User*>::iterator itr;
		for (itr=begin(this->_userLogList); itr != end(this->_userLogList); ++itr) {
			if(_nick == *(*itr).getNick())
				_userLogList.erase(itr);
}
