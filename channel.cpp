#include "channel.hpp"


		/*  Constructors and Deconstructors  */
//===================================================//


Channel::Channel(std::string newName) : _channelName(newName), _maxClients(60), _countClients(0) {
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
}

void	Channel::addBanned(User* toBan)
{
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
		if (((*start))->getNick().compare(user->getNick()) == 0)
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


void	Channel::deleteUser(std::string _nick)
{
		std::string str;

		std::vector<User*>::iterator itr;
		for (itr=begin(this->_userLogList); itr != end(this->_userLogList); itr++)
		{
			if(_nick == (*itr)->getNick())
				_userLogList.erase(itr);
		}
}




			/*    Checkers     */
//===================================================//


/* Finds if Nick in Channel and returns a Pointer to User Obj or NULL */
User*	Channel::isUserinChannelbys(std::string _nick)
{
		std::vector<User*>::iterator itr;
		for (itr=begin(this->_userLogList); itr != end(this->_userLogList); itr++)
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
		for (itr=begin(this->_userLogList); itr != end(this->_userLogList); itr++)
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
		for (itr=begin(this->_userLogList); itr != end(this->_userLogList); itr++)
		{
			if(_nick == (*itr)->getNick())
				return(1);
		}
		return (0);
}




/* Finds a User Obj Pointer from the _userLogList return 1 or 0 */
int		Channel::is_User_Operator(User* user)
{



	// Protection: Check if the _userLogList is empty   Not Needed Now!!!
	/* if (this->_channelList.size() == 0)
	{

		return;
	} */

		std::vector<User*>::iterator itr;
		for (itr=begin(this->_userLogList); itr != end(this->_userLogList); itr++)

		{
			if(user == *itr)
				return(1);
		}
		return (0);
}





/*    Not Checked   */
//=====================================================================
//=====================================================================
std::string	Channel::findAllUsers() {
		std::string str;

		std::vector<User*>::iterator itr;
		for (itr = begin(this->_userLogList); itr != end(this->_userLogList); itr++)
		{
			//std::cout << "Found user in channel list: " << (*itr)->getNick() << std::endl;
			if (this->ifOperator((*itr)->getNick()) == 1) {
				str += '@';
			}
			std::string temp = (*itr)->getNick();
			str += temp + " ";
			// temp.clear();
		}
		// std::cout << "String from all users is: " << str << std::endl;
		return (str);
}


bool	Channel::ifBanned(User* ifBan) {
	std::vector<User*>::iterator itr;
	for (itr=begin(this->_bannedList); itr != end(this->_bannedList); itr++) {
		if ((char *)ifBan->getHost() == *(*itr)->getHost())
			return true;
	}
	std::cerr << "User not found as banned" << std::endl;
	return false;
}

int		Channel::ifOperator(std::string _nick)
{
		std::string str;

		std::vector<User*>::iterator itr;
		for (itr=begin(this->_operatorList); itr != end(this->_operatorList); itr++)
		{
			if(_nick == (*itr)->getNick())
				return(1);
		}
		return (0);
}


/* bool	Channel::ifIsOperator(User* checkUser)
{
	std::vector<User *>::iterator itr = ::find(_operatorList.begin(), _operatorList.end(), checkUser);
	if (itr != _operatorList.end())
		return true;
	return false;
} */


// commented out for adding new iterator and testing, 23.02.
/* User*	Channel::get_user_if_in( std::string& _nick)
{
	std::vector<User *>::iterator start = this->_userLogList.begin();
	std::vector<User *>::iterator end = this->_userLogList.end();
	std::string *test = _nick;

	while (start != end)
	{
		if ((*start)->getNick()->compare(test) == 0)
		{
			std::cout << (*start)->testing << std::endl;
			return ((*start));
		}
		start++;
	}
	return (NULL);
} */



// Currently commented out for bugfixing: has to be revisited 
/* void Channel::notify_others(const std::string& msg, User* skip)
{
	std::vector<User*>::iterator start = this->_userLogList.begin();
	std::vector<User*>::iterator end = this->_userLogList.end();

	while (start != end)
	{
		if ((*start)->getNick()->compare(*skip->getNick()) == 0)
		{
			continue;
		}
		(*start)->reply(msg.c_str());
		start++;
	}
}  */

  
  
  
  
void Channel::notify_others(std::string cmd_name, const std::string& msg, User* skip)
{
	std::string tosend = ":" + skip->getPrefix() + " " + cmd_name + " " + this->getName() + " " + msg;

	std::vector<User*>::iterator itr;
	for (itr=begin(this->_userLogList); itr != end(this->_userLogList); itr++) {
		if (skip->getNick() != (*itr)->getNick())
			(*itr)->replyLight(tosend.c_str());
	}
}


