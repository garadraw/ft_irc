#include "channel.hpp"

const std::string& Channel::getName()
{
	return (this->channelName);
}

size_t Channel::getMaxClients() const
{
	return (this->max_clients;)
}

size_t	Channel::getClientCount() const
{
	return (this->count_clients);
}

void	Channel::addUser(User* user)
{
	this->_userLogList.push_back(user);
}

void Channel::notify_others(const std::string& msg, User* skip)
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
}