#include "user.hpp"
#include "server.hpp"
#include "channel.hpp"

#include "CommanHandler.hpp"

					/*        REPLY               */

void User::reply(/*const std::string& reply_msg*/ const char* reply_msg)
{
	std::string str_reply;
	str_reply.assign(reply_msg);

	std::cout << "char* accepted in function reply: " << str_reply << std::endl;
	std::string sendMessage = ":" + getPrefix() + " " + str_reply + "\r\n";
	// write(":" + getPrefix() + " " + reply_msg);
	send(_fd, sendMessage.c_str(), sendMessage.size(), 0);
}

/* replyLight is similar to reply(), but does not add a prefix, 
and allows sending custom messages back to client */
void User::replyLight(const char* reply_msg)
{
	std::string str_reply;
	str_reply.assign(reply_msg);

	std::cout << "char* accepted in function replyLight: " << str_reply << std::endl;
	std::string sendMessage = str_reply + "\r\n";
	// write(":" + getPrefix() + " " + reply_msg);
	send(_fd, sendMessage.c_str(), sendMessage.size(), 0);
}




					/*        KICK               */

// format: KICK <channel> <user> [<comment>] // COMMENTED OUT BY MADIS FOR DEBUGGING 23.02.
 void User::execute_kick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{

	// Probably handled by parser and command hadler
	// fill in reasons - commented out by Madis during debugging 
	if (args.size() < 2 )
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return;
	}
	// if (args.size() >= 3 && (args[2][0] != ':' || args[2].size() >= 1))
	// {
	// 	std::vector<std::string>::iterator start = args.begin() + 2;
	// 	std::vector<std::string>::iterator end = args.end();

	// 	while (start != end)
	// 	{
	// 		reason.append((*start) + " ");
	// 		start++;
	// 	}
	// }


	const std::string channel_name = args[0];
	std::string target = args[1];
	std::string reason = "";

		// Find Channel in Server Channel List and return it's Channel Obj Pointer
		if (user->_server->findChannel(channel_name) == NULL)
		{
			user->reply("Channel not in Channel list");
		}
		Channel* foundChannel = user->_server->findChannel(channel_name);
		
		
		
		// Find if user that wants to kick someone is in same channel 
		//  Find First User in Channel::UsersinChannelList or in User::_channelList
		// Compare channels


		// We know have Channel Pointer and First User Pointer

		//  Find First User by User Obj Pointer in Channel::UserList and return 1 if True, 0 if False
		if (foundChannel->isUserinChannelbyp(user) == NULL)
		{
			//reply("First User is not Found in Channel");
			user->reply(ERR_NOTONCHANNEL(user->getNick(), foundChannel._channelName));
		}

/* 
		//  Find Channel in User::ChanneList    Optional
		Channel*	User::findUserinChannel(std::string name) {
		
		if(this->findUserinChannel(this) == NULL)
		{
			reply(User not in Channel)
		}
*/

		//  Find Second User by Nick in Channel and return a pointer to Second User Object
		if (foundChannel->isUserinChannelbys(target) == NULL)
		{
			//reply("Second User/Target User is not Found in Channel");
			// user->reply(ERR_NOSUCHNICK(target, channel_name));
			// user->reply(ERR_USERNOTINCHANNEL(target, channel_name));
		}
		else if (foundChannel->isUserinChannelbys(target) != NULL)
		{
			User* secondUser = foundChannel->isUserinChannelbys(target);
			// We know the Channel Pointer, Pointer for the First User and Pointer for Second User

			//Find First User in Channel Operator List
			if (foundChannel->is_User_Operator(user) == 0)
			{
				//reply("to First User that he cannot Kick second User");
				// user->reply(ERR_CHANOPRIVSNEEDED(user->getNick(), foundChannel));
			}
			else if (foundChannel->is_User_Operator(this) == 1)
			{
				foundChannel->deleteUser(target);
				secondUser->delete_Channel(foundChannel);
				
			}
		}

		// TODO Notify All Users in Channel that User2 has been kicked
		// foundChannel->notify_others(RPL_KICK(user, channel_name, target, reason), this);


	std::cout << target << " has been kicked from channel " << channel_name << ".\n";
}



						/*        BAN               */

// format: BAN <channel> <user> [<comment>] 
 void User::execute_ban_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{

	// Probably handled by parser and command hadler
	// fill in reasons - commented out by Madis during debugging 
	if (args.size() < 2 )
	{
		// user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return;
	}


	// if (args.size() >= 3 && (args[2][0] != ':' || args[2].size() >= 1))
	// {
	// 	std::vector<std::string>::iterator start = args.begin() + 2;
	// 	std::vector<std::string>::iterator end = args.end();

	// 	while (start != end)
	// 	{
	// 		reason.append((*start) + " ");
	// 		start++;
	// 	}
	// }


	const std::string channel_name = args[0];
	std::string target = args[1];
	std::string reason = "";

		// Find Channel in Server Channel List and return it's Channel Obj Pointer
		if (user->_server->findChannel(channel_name) == NULL)
		{
			// user->reply("Server: Channel not in Channel list");
		}
		Channel* foundChannel = user->_server->findChannel(channel_name);
		
		
		
		// Find if user that wants to kick someone is in same channel 
		//  Find First User in Channel::UsersinChannelList or in User::_channelList
		// Compare channels


		// We know have Channel Pointer and First User Pointer

		//  Find First User by User Obj Pointer in Channel::UserList and return 1 if True, 0 if False
		if (foundChannel->isUserinChannelbyp(user) == NULL)
		{
			//reply("First User is not Found in Channel");
			// user->reply(ERR_NOTONCHANNEL(user->getNick(), foundChannel._channelName));
		}

/* 
		//  Find Channel in User::ChanneList    Optional
		Channel*	User::findUserinChannel(std::string name) {
		
		if(this->findUserinChannel(this) == NULL)
		{
			reply(User not in Channel)
		}
*/

		//  Find Second User by Nick in Channel and return a pointer to Second User Object
		if (foundChannel->isUserinChannelbys(target) == NULL)
		{
			//reply("Second User/Target User is not Found in Channel");
			// user->reply(ERR_NOSUCHNICK(target, channel_name));
			// user->reply(ERR_USERNOTINCHANNEL(target, channel_name));
		}
		else if (foundChannel->isUserinChannelbys(target) != NULL)
		{
			User* secondUser = foundChannel->isUserinChannelbys(target);
			// We know the Channel Pointer, Pointer for the First User and Pointer for Second User

			//Find First User in Channel Operator List
			if (foundChannel->is_User_Operator(user) == 0)
			{
				//reply("to First User that he cannot Ban second User");
				// user->reply(ERR_CHANOPRIVSNEEDED(user->getNick(), foundChannel));
			}
			else if (foundChannel->is_User_Operator(user) == 1)
			{
				foundChannel->addBanned(secondUser);
				foundChannel->deleteUser(target);
				secondUser->delete_Channel(foundChannel);
				
			}
		}

		// Notify All Users in Channel that User2 has been Banned
		// foundChannel->notify_others(RPL_BAN(user, channel_name, target, reason), this);


	std::cout << target << " has been Banned and Kicked from channel " << channel_name << ".\n";
}







				/*        JOIN               */

void User::execute_join_cmd(User* user, std::string& cmd_name, std::vector<std::string> args)
{
	// std::cout<<"it reached the Join Command" << std::endl;
	if (this->isRegistered() == false) {
		this->reply(ERR_NOTREGISTERED(this->getNick()));
		return ;
	}

	if(this->_server->findChannel(args[0]) == NULL)
	{
		if (args[0].at(0) != '#')
		{
			replyLight("Wrong input for channel name, try with #");
			return ;
		}

		//apparently not needed to remove the # ?
		// std::string clearName = args[0].substr(1);

		Channel* newChannel = new Channel(args[0]);
		this->_server->addChannel(newChannel);
		newChannel->addCreator(this);
		newChannel->addUser(this);
		this->reply(RPL_JOIN(this->getNick(), newChannel->getName()));
		this->_channelList.push_back(newChannel);

		//this part is for making client recognize operators: / has to be sent on login
		std::string allUserList = newChannel->findAllUsers();
		std::string reply_construct = ":" + this->_server->_serverName + " 353 " + this->getNick() + " = " + newChannel->getName() + " :" + allUserList;
		this->replyLight(reply_construct.c_str());

		newChannel->_countClients++;
	}
	else {
		Channel* foundChannel = this->_server->findChannel(args[0]);
		if (foundChannel->ifBanned(this) == true)
		{
			this->reply(ERR_BANNEDFROMCHAN(args[0]));
		}
		else
		{
			// maximum of 20 channels per connection
			if (foundChannel->getClientCount() >= foundChannel->getMaxClients())
			{
				this->reply(ERR_CHANNELISFULL(this->getNick(), args[0]));
				return ;
			}
			if (this->_channelList.size() >= 20)
			{
				this->reply(ERR_TOOMANYCHANNELS(this->getNick(), args[0]));
				return ;
			}
			if (foundChannel->ifJoined(this->getNick()) == 1) {
				this->reply(ERR_USERONCHANNEL(this->getNick(), foundChannel->getName()));
				return ;
			}
			foundChannel->addUser(this);
			this->_channelList.push_back(foundChannel);
			std::string allUserList = foundChannel->findAllUsers();
			std::string reply_construct = ":" + this->_server->_serverName + " 353 " + this->getNick() + " = " + foundChannel->getName() + " :" + allUserList;
			this->replyLight(reply_construct.c_str());
			foundChannel->_countClients++;
		}
	}
}


	


// PRIVMSG <receiver> <text to be sent>
void User::execute_privmsg_cmd(std::string cmd_name, User* user, std::vector<std::string> args)
{
	if (args[0].empty() || args.size() < 2 || args[1].empty())
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), "PRIVMSG"));
		return;
	}

	std::string receiver = args[0];
	std::string msg;

	std::vector<std::string>::iterator start = args.begin();
	start++;
	std::vector<std::string>::iterator end = args.end();

	// read in message args
	while(start != end)
	{
		msg.append(*start);
		msg.append(" ");
		start++;
	}
	// check if msg starts with ':'
	if (msg[0] == ':')
		msg = msg.substr(1);
	
	std::cout << "Receiver hat Inhalt: " << receiver << std::endl;
	if (receiver[0] == '#') // #: send to all in channel, here: check if channel exists
	{
		std::string channel_name = receiver.substr(0);
		Channel* channel = user->get_channel_if_in(channel_name); // is calling user in that channel he wants to send to?

		if (channel == NULL)
		{
			user->reply(ERR_NOSUCHCHANNEL(user->getNick(), channel_name));
			return ;
		}
		std::cout << "HIIIII\n";
		channel->notify_others(cmd_name, msg, user);
		return ;
	}
	// receiver not on server
	User* target_user = user->_server->findByNick(receiver);
	if (target_user == NULL)
	{
		user->reply(ERR_NOSUCHNICK(receiver));
		return;
	}
	// acual sending the priv msg (can make one write/send function out of it for other uses)
	std::string priv_msg = RPL_PRIVMSG(user->getPrefix(), receiver, msg);
	std::string msg_buf = priv_msg + "\r\n";
	if (send(target_user->_fd, msg_buf.c_str(), msg_buf.length(), 0) < 0)
	{
		std::cerr << "Error while sending message to client." << std::endl;
	}
}


			/* 			NICK			*/
      
void User::execute_nick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.size() == 0)
	{
		user->reply(ERR_NONICKNAMEGIVEN(user->getNick())); // removed c_str() appendex
		return ;
	}

	std::string nickname = args[0];

	if (user->_server->findByNick(nickname) != NULL)
	{
		user->reply(ERR_NICKNAMEINUSE(/*convert_to_string*/(user->getNick())));
		return ;
	}

	user->setNick(nickname);
	user->setRealname(nickname);
	user->reply(RPL_WELCOME(nickname));

	//Notify all Users in All channels in which the User is in about the nicknamechange
}





// -----------------------
/* Currently only returns the nickname, because other data has to be added/checked still */
std::string User::getPrefix() {
/* 	std::string return_msg; // = (*this->getNick());
	char* buf = this->_host;
	std::string hostname;
	hostname.assign(buf);
	std::string buffer = this->getNick(); */

	char* buf = this->_host;
	std::string hostname;
	hostname.assign(buf);

	std::string return_msg = this->getNick();

	/* :<nickname>!<username>@host CMD <target> \r\n */

	if (/* this->getUsername().empty() */this->getNick().empty() == false) {
		return_msg += "!" + /* this->getUsername() */ this->getNick();
		if (/* *this->getHost().empty() */ hostname.empty() == false)
		{
			return_msg += "@" + hostname;
		}
	}
	return(return_msg);
 }



/*
*	Use of the extended prefix (['!' <user> ] ['@' <host> ]) must
	not be used in server to server communications and is only
	intended for server to client messages in order to provide
	clients with more useful information about who a message is
	from without the need for additional queries.
	-> calling start in server leading to here.

/*
* format: JOIN <channel>{,<channel>} [<key>{,<key>}]
*/




/* void User::write_msg(const std::string& msg) const
{
	std::string new_msg = msg + "\r\n"; // per documentation definition line in irc end with \r
	if (send(_fd, new_msg.c_str(), new_msg.length(), 0) < 0) // c_str =copy of the string is sent
	{
		std::cerr << "error when notifying memebers of channell about someone joining\n";
	}
} */



void	execute_mode_cmd(std::string cmd_name, std::vector<std::string> cmd_args) {
	std::cout << "Reached mode command with cmd_name " << cmd_name << " and arguments " << cmd_args[0] << " and " << cmd_args[1] << std::endl;
}




// format: QUIT (:)[<quitmsg>] // COmmanted out by Madsi for debugging on 23.02.
/* void User::execute_quit_cmd(const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.empty() == true)
	{
		reason = "no reason";
	}
	else
	{
		reason = args[0];
		if (reason[0] == ':')
		{
			reason = reason.substr(1);
		}	
	}
	this->reply(RPL_QUIT(this->getPrefix(), reason));


 // ---------------------------------------------------------------------

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
} */


// format: PING <server1> [<server2>]
/* void User::execute_ping_cmd(const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.size() == 1)
	{
		if (!strcmp((const char *)args[0][0], (const char *)_host)) //comparing args[0] to servername, is _host the servername? ANswer: no, it is not, the server name is _name unders the server, and the _host is the IP/host of a connected client
		{
			reply(("PONG " + args[0]).c_str());
		}
		else
		 reply(ERR_NOSUCHSERVER(args[0]));
	}
} */

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

// std::string convert_to_string(char* string)
// {
// 	std::string str; // = (*this->getNick());
// 	str.assign(string);

// 	return (str);
// }


// // format: NICK <nickname>
// void User::execute_nick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
// {
// 	if (args.size() == 0)
// 	{
// 		user->reply(ERR_NONICKNAMEGIVEN(user->getNick())); // removed c_str() appendex
// 		return ;
// 	}

// 	std::string nickname = args[0];

// 	if (user->_server->findByNick(nickname) != NULL)
// 	{
// 		user->reply(ERR_NICKNAMEINUSE(/*convert_to_string*/(user->getNick())));
// 		return ;
// 	}

// 		user->setNick(nickname);
// 		std::cout << "Nickname set in NICK cmd to: " << this->getNick() << std::endl;
// 		// user->setRealname(nickname);
// 		user->reply(RPL_WELCOME(nickname));
// }

// format: PASS <password>
void User::execute_pass_cmd(User* user, const std::string& cmd_name, std::string args)
{
	if (args.size() == 0)
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return ;
	}
	if (user->isRegistered() == true)
	{
		user->reply(ERR_ALREADYREGISTRED(user->getNick()));
		return ;
	}
	std::string _newPassword = args;
	if (user->_server->getPass() == _newPassword)
	{
		this->setRegistered();
		user->replyLight("Successfully registered");
		return ;
	}
	else
		user->replyLight("Wrong password");
	return ;
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
	/* if (user->isRegistered())
	{
		user->reply(ERR_ALREADYREGISTRED(user->getNick()));
		return;
	} */

	/* if (args.size() < 4)
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), "USER"));
		return;
	} */

	user->setUsername(args[0]);
	std::cout << "The username is set to: " << this->getUsername() << std::endl;
	user->setRealname(args[3]);
	std::cout << "The realname is set to: " << this->getRealname() << std::endl;
	// user->reply(RPL_WELCOME(user->getNick()));
}
