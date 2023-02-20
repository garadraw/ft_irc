#include "user.hpp"

// -----------------------
/* Currently only returns the nickname, because other data has to be added/checked still */
std::string User::getPrefix() const
 {
	std::string return_msg = this->getNick();

	/* if (this->_nick.size() != 0)
	{
		return_msg += "!" + this->getUsername()
		if (this->_hostname.size() != 0)
		{
			return_msg += "@" + this->getHostname(); // no hostname func yet
		}
	} */
	return(return_msg);
 }

/*
*	Use of the extended prefix (['!' <user> ] ['@' <host> ]) must
	not be used in server to server communications and is only
	intended for server to client messages in order to provide
	clients with more useful information about who a message is
	from without the need for additional queries.
	-> calling start in server leading to here.
*/
void User::reply(const std::string& reply_msg)
{
	std::cout << "This message from reply function under User object\n";
	/* write(":" + getPrefix() + " " + reply_msg); */
}

/*
* format: JOIN <channel>{,<channel>} [<key>{,<key>}]
*/
void User::execute_join_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.empty() == true)
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name ))
		return ;
	}
	
	std::string channel_name = args[0];
	if (args.size() > 1)
	{
		std::string password = args[1];
	}

	// maximum of 20 channels per conenction
	if (user->_channels.size() >= 20)
	{
		user->reply(ERR_TOOMANYCHANNELS(user->getNick(), channel_name));
		return ;
	}

	Channel* channel = _server->getChannel(channel_name); // still needs _server in user
	if (!channel)
	{
		_server->createChannel(some params) // need create channel in server
	}

	if (channel->getClientsCount() >= channel->getMaxClients())
	{
		user->reply(ERR_CHANNELISFULL(user->getNick(), channel_name));
		return ;
	}

	if (channel->getPWD().compare(password) != 0)
	{
		user->reply(ERR_BADCHANNELKEY(user->getNick(), channel_name));
		return ;
	}
	// actual joining
	channel->addUser(user);
	_channels.push_back(channel);

	std::string user_list;
	std::vector<User*>::iterator start = channel->_userLogList.begin();
	std::vector<User*>::iterator end = channel->_userLogList.end();

	while (start != end)
	{
		user_list.append((*start)->getNick());
		start++;
	}

	reply(RPL_NAMEREPLY(user->getNick(), channel->getName(), user_list));
	reply(RPL_ENDOFNAMES(user->getNick(), channel->getName()));

	channel->notify_others(RPL_JOIN(getPrefix(), channel->getName()));
	std::cout << user->getNick() << " has joined channel " << channel->getName() << std::endl;
}

void User::write_msg(const std::string& msg) const
{
	std::string new_msg = msg + "\r\n"; // per documentation definition line in irc end with \r
	if (send(_fd, new_msg.c_str(), new_msg.length(), 0) < 0) // c_str =copy of the string is sent
	{
		std::cerr << "error when notifying memebers of channell about someone joining\n";
	}
}
