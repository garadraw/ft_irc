#include "channel.hpp"


		/*  Constructors and Deconstructors  */
//===================================================//

Channel::Channel(std::string newName) : _countClients(0), _userLogList(), _server(), _channelName(newName), _maxClients(60), _creatorUser(), _operatorList(), _bannedList() {
	std::cout << "New channel created with name " << _channelName << std::endl;
}

Channel&	Channel::operator=(const Channel &src) {
	this->_channelName = src._channelName;
	this->_bannedList = src._bannedList;
	this->_operatorList = src._operatorList;
	return (*this);
}

Channel::~Channel() {
	std::cout << "Channel destroyed" << std::endl;
}



		/*   Getters and Setters    */
//===================================================//

void	Channel::addCreator(User* creator) {
	this->_creatorUser = creator;
	this->setOperator(creator);
}

std::vector<User*>	Channel::getUsers() {
	return (this->_userLogList);
}

std::vector<User*>	Channel::getOperators() {
	return (this->_operatorList);
}

void	Channel::setOperator(User* newOp) {
	this->_operatorList.push_back(newOp);
}

const std::string Channel::getName()
{
	return (this->_channelName);
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
	std::cout << "This nick is stored on the userLogList: " << user->getNick() << std::endl;
}

void	Channel::addBanned(User* toBan)
{
	this->_bannedList.push_back(toBan);
}

/* Removes a user from a channel, and removes channel if last user has left. Notifies other clients of action. */
void	Channel::deleteUser(std::string _nick)
{
	std::string str;
	std::vector<User*>::iterator itro;
	for (itro=this->_operatorList.begin(); itro != this->_operatorList.end(); itro++)
	{
		if (_nick == (*itro)->_nick)
		{
			_operatorList.erase(itro);
			break ;
		}
	}
	std::vector<User*>::iterator itr;
	for (itr=this->_userLogList.begin(); itr != this->_userLogList.end(); itr++)
	{
		if (_nick == (*itr)->_nick) {
			_userLogList.erase(itr);
			break ;
		}
	}
	if (_userLogList.empty() == false) {
		std::string allUserList = this->findAllUsers();
		std::string reply_construct = ":" + this->_server->_serverName + " 353 " + (*itr)->getNick() + " = " + this->getName() + " :" + allUserList;
		(*itr)->replyLightAll(reply_construct.c_str(), this);
	}
	if (_userLogList.empty()) {
		std::cout << (*(this->_server->_channelList.begin()))->getName() << std::endl;
		if (!this->_server->_channelList.empty()) {
			std::vector<Channel*>::iterator	iterc = this->_server->_channelList.begin();
			long unsigned int i = 0;
			while (i < this->_server->_channelList.size())
			{
				if (this->getName() == this->_server->_channelList.at(i)->getName()) {
					
					this->_server->_channelList.erase(iterc);
					break ;
				}
				iterc++;
				i++;
			}
		}
		delete this;
	}
}

void Channel::partUser(std::string _nick) {
		
		std::vector<User*>::iterator itro;
		for (itro=this->_operatorList.begin(); itro != this->_operatorList.end(); itro++)
		{
			if (_nick == (*itro)->_nick) {
				_operatorList.erase(itro);
				break ;
			}
		}
		std::vector<User*>::iterator itr;
		for (itr=this->_userLogList.begin(); itr != this->_userLogList.end(); itr++)
			{
				if (_nick == (*itr)->_nick) {
					_userLogList.erase(itr);
					break ;
				}
			}
		if (_userLogList.empty()) {
			std::cout << (*(this->_server->_channelList.begin()))->getName() << std::endl;
			if (!this->_server->_channelList.empty()) {
				std::vector<Channel*>::iterator	iterc = this->_server->_channelList.begin();
				unsigned long int i = 0;
				while (i < this->_server->_channelList.size())
				{
					if (this->getName() == this->_server->_channelList.at(i)->getName()) {
						
						this->_server->_channelList.erase(iterc);
						break ;
					}
					iterc++;
					i++;
				}
			}
			delete this;
		}
}


			/*    Checkers     */
//===================================================//


/* Finds if Nick in Channel and returns a Pointer to User Obj or NULL */
User*	Channel::isUserinChannelbys(std::string _nick)
{
		std::vector<User*>::iterator itr;
		for (itr=this->_userLogList.begin(); itr != this->_userLogList.end(); itr++)
		{
			if(_nick == (*itr)->getNick())
				return(*itr);
		}
		return (NULL);
}

/* Finds if User Obj Pointer in Channel and returns a Pointer to User Obj or NULL */
User*	Channel::isUserinChannelbyp(User* user)
{
		std::vector<User*>::iterator itr;
		for (itr=this->_userLogList.begin(); itr != this->_userLogList.end(); itr++)
		{
			if(user == *itr)
				return(*itr);
		}
		return (NULL);
}

/* Not Needed anymore, Use Channel::isUserinChannel */
int		Channel::ifJoined(std::string _nick)
{
		std::string str;

		std::vector<User*>::iterator itr;
		for (itr=this->_userLogList.begin(); itr != this->_userLogList.end(); itr++)
		{
			if(_nick == (*itr)->getNick())
				return(1);
		}
		return (0);
}




/* Finds a User Obj Pointer from the _userLogList return 1 or 0 */
int		Channel::is_User_Operator(User* user)
{
	// Protection: Check if the _userLogList is empty
	/* if (this->_channelList.size() == 0) {
		return;
	} */
		std::vector<User*>::iterator itr;
		for (itr=this->_operatorList.begin(); itr != this->_operatorList.end(); itr++)
		{
			if(user == *itr)
				return(1);
		}
		return (0);
}


		/*    Utilities   */
//=====================================================================

std::string	Channel::findAllUsers()
{
	std::string str;
	std::vector<User*>::iterator itr;
	for (itr = this->_userLogList.begin(); itr != this->_userLogList.end(); itr++)
	{
		if (this->ifOperator((*itr)->getNick()) == 1) {
			str += '@';
		}
		std::string temp = (*itr)->getNick();
		str += temp + " ";
	}
	return (str);
}

bool	Channel::ifBanned(User* ifBan) {
	std::vector<User*>::iterator itr;
	for (itr=this->_bannedList.begin(); itr != this->_bannedList.end(); itr++) {
		if ((char *)ifBan->getHost() == *(*itr)->getHost())
			return true;
	}
	std::cerr << "User not found as banned" << std::endl;
	return false;
}

int	Channel::ifOperator(std::string _nick)
{
		std::string str;

		std::vector<User*>::iterator itr;
		for (itr=this->_operatorList.begin(); itr != this->_operatorList.end(); itr++)
		{
			if(_nick == (*itr)->getNick())
				return(1);
		}
		return (0);
}

void Channel::notify_others(std::string cmd_name, const std::string& msg, User* skip)
{
	std::string tosend = ":" + skip->getPrefix() + " " + cmd_name + " " + this->getName() + " " + msg;

	std::vector<User*>::iterator itr;
	for (itr=this->_userLogList.begin(); itr != this->_userLogList.end(); itr++) {
		if (skip->getNick() != (*itr)->getNick())
			(*itr)->replyLight(tosend.c_str());
	}
}

void Channel::deleteOperator(User* target)
{
	std::vector<User*>::iterator start = this->_operatorList.begin();
	std::vector<User*>::iterator end = this->_operatorList.end();
	while (start != end)
	{
		if (target->getNick() == (*start)->getNick())
		{
			this->_operatorList.erase(start);
			return;
		}
		start++;
	}
}



