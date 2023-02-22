#include "user.hpp"
#include "server.hpp"
#include "channel.hpp"


#include "commanHandler.hpp"

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
	if(this->_server.findChannel(args) == NULL)
	{
		Channel* newChannel = new Channel(args);
		this->_server.addChannel(newChannel);
		newChannel->addUser(this);
		this->_channels.push_back(newChannel);
		newChannel->addCreator(this);
	}
	else
	{
		Channel* foundChannel = this->_server.findChannel(args);
		if (foundChannel.ifBanned(this) == true)
		{
			this.reply(ERR_BANNEDFROMCHAN(foundChannel->getName()));
		}
		else
		{
			// maximum of 20 channels per conenction
			if (foundChannel->getClientCount() >= foundChannel->getmaxClients())
			{
				this->reply(ERR_CHANNELISFULL(this->getNick(), foundChannel._channelName));
  ///TODO		return ;
			}
			if (this->_channels.size() >= 20)
			{
				this->reply(ERR_TOOMANYCHANNELS(this->getNick(), foundChannel));
				return ;
			}
			foundChannel->addUser(this);
			this->_channels.push_back(foundChannel);
			this.reply(RPL_NAMREPLY(foundChannel.findAllUsers()) //Need to Come back
			_countClients++;
		}
	}

					00      01
// format: KICK <channel> <user> [<comment>]
void User::execute_kick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	if(this->_channel.ifOperator(*args[1]) == NULL)
	{
		
	}

}



void User::write_msg(const std::string& msg) const
{
	std::string new_msg = msg + "\r\n"; // per documentation definition line in irc end with \r
	if (send(_fd, new_msg.c_str(), new_msg.length(), 0) < 0) // c_str =copy of the string is sent
	{
		std::cerr << "error when notifying memebers of channell about someone joining\n";
	}
}
					00      01
// format: KICK <channel> <user> [<comment>]
void User::execute_kick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.size() < 2 )
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return;
	}

	std::string channel_name = args[0];
	std::string target = args[1];
	std::string reason = "";

	// fill in reasons
	if (args.size() >= 3 && (args[2][0] != ':' || args[2].size() >= 1))
	{
		std::vector<std::string>::iterator start = args.begin() + 2;
		std::vector<std::string>::iterator start = args.end();

		while (start != end)
		{
			reason.append((*start) + " ");
			start++;
		}
	}

	// check if user that wants to kick someone is in same channel
	Channel* foundChannel = user->get_if_in_channel(const std::string& channel_name);
	if (foundChannel == NULL)
	{
		user->reply(ERR_NOTONCHANNEL(user->getNick(), foundChannel._channelName));
		return;
	}

	// check if user is admin
	if (foundChannel.ifOperator(this) == NULL) // overload or via getName()
	{
		user->reply(ERR_CHANOPRIVSNEEDED(user->getNick(), foundChannel));
		return;
	}

	// check if target-user is in channel
	User* target_user = channel->get_user_if_in(target);
	if (target_user == NULL)
	{
		user->reply(ERR_NOSUCHNICK(target, channel_name));
		user->reply(ERR_USERNOTINCHANNEL(target, channel_name));
		return;
	}

	// actual kicking

	channel->notify_others(RPL_KICK(user, channel_name, target, reason), this);

	target_user->deleteChannel(target_user->_get_channel_if_in(channel_name));

	foundChannel->deleteUser(target)

	
	std::cout << target << " has been kicked from channel " << channel_name << ".\n";
}

// format: QUIT (:)[<quitmsg>]
void User::execute_quit_cmd(const std::string& cmd_name, std::vector<std::string> args)
{
	//send quit msg
	std::vector<Channel *> channel = getChannels();
	for(int i = 0;  i < getChannels().size(); i++)
	{
		if (args.size() == 0)
		{
			channel[i]->notify_others("QUIT", this);
			channel[i]->delete_user(this);
			
		}
		else if (args.size() > 0)
		{
			std::string quitmsg = args[0];
			if (quitmsg.at(0) == ':')
			{
				quitmsg = quitmsg.substr(1);
			}
			channel[i]->notify_others("QUIT :" + args[0], this);
			channel[i]->delete_user(this);
		}
	}
	setFd(-1);
	_isRegistered = false;
	//killUser(this, "User hast quit"); //When command is implemented, we only need to call it
}


// format: PING <server1> [<server2>]
void User::execute_ping_cmd(const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.size() == 1)
	{
		if (!strcmp((const char *)args[0][0], (const char *)_host)) //comparing args[0] to servername, is _host the servername?
		{
			reply("PONG " + args[0]);
		}
		else
		 reply(ERR_NOSUCHSERVER(args[0]));
	}
}

// format: INVITE <nickname> <channel>
// void User::execute_invite_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
// {
// 	// check if user is operator operator rights of the channel
// 	if (args.size() < 2)
// 		reply(ERR_NEEDMOREPARAMS(_nick, cmd_name));
// 	else if (args.size() == 2)
// 	{
// 		//is nickname existing
// 			//using the server function to find the nickname

// 		//is channel existing
// 			//using the server function to find the channel

// 		//is the inviter channel operator
// 	}
// }

// format: NICK <nickname>
void User::execute_nick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.size() == 0)
	{
		user->reply(ERR_NONICKNAMEGIVEN(user->getNick()));
		return;
	}

	std::string nickname = args[0];

	if (user->_server->findByNick(nickname) != NULL)
	{
		user->reply(ERR_NICKNAMEINUSE(user->getNick()));
		return;
	}

	user->setNick(nickname);
	user->reply(RPL_WELCOME(nickname));
}

/*
* format: USER <username> <hostname> <servername> <realname>
* ***********************************************************
* used in communication between servers to indicate new user
* arriving on IRC, since only after both USER and NICK have been
* received from a client does a user become registered.
*/
void User::execute_user_cmd(User* user, std::vector<std::string> args)
{
 if (user->isRegistered())
 {
	user->reply(ERR_ALREADYREGISTERED(user->getNick()));
	return;
 }

 if (args.size() < 4)
 {
	user->reply(ERR_NEEDMOREPARAMS(user->getNick()), "USER");
	return;
 }

 user->setUsername(args[0]);
 user->setRealname(args[3]);
 user->reply(RPL_WELCOME(user->getNick()));

}
