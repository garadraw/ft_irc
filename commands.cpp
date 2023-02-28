#include "user.hpp"
#include "server.hpp"
#include "channel.hpp"
#include "CommanHandler.hpp"

/*        REPLY        */
void User::reply(/*const std::string& reply_msg*/ const char* reply_msg)
{
	std::string str_reply;
	str_reply.assign(reply_msg);

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

	// std::cout << "char* accepted in function replyLight: " << str_reply << std::endl;
	std::string sendMessage = str_reply + "\r\n";
	send(_fd, sendMessage.c_str(), sendMessage.size(), 0);
}

/* replyLight is similar to reply(), but does not add a prefix, 
and allows sending custom messages back to client */
void User::replyLightAll(const char* reply_msg, Channel* activeChannel)
{
	std::vector<User *>::iterator itr;
	std::vector<User *>::iterator start = activeChannel->_userLogList.begin();
	std::vector<User *>::iterator end = activeChannel->_userLogList.end();
	std::string str_reply;
	str_reply.assign(reply_msg);

	std::string sendMessage = str_reply + "\r\n";

	for (itr = start; itr != end; itr++) {
		send((*itr)->getFd(), sendMessage.c_str(), sendMessage.size(), 0);
	}
}

/*        KICK        */
// format: KICK <channel> <user> [<comment>]
 void User::execute_kick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	// Probably handled by parser and command handler
	// fill in reasons - commented out by Madis during debugging 
	if (args.size() < 2 )
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return ;
	}

	const std::string channel_name = args[0];
	std::string target = args[1];
	std::string reason = "";

		// Find Channel in Server Channel List and return it's Channel Obj Pointer
		if (user->_server->findChannel(channel_name) == NULL)
		{
			user->replyLight("Channel does not exist");
			return ;
		}
		Channel* foundChannel = user->_server->findChannel(channel_name);
		
		// Find if user that wants to kick someone is in same channel 
		// Find First User in Channel::UsersinChannelList or in User::_channelList
		// Compare channels

		// We know have Channel Pointer and First User Pointer

		// Find First User by User Obj Pointer in Channel::UserList and return 1 if True, 0 if False
		if (foundChannel->isUserinChannelbyp(user) == NULL)
		{
			user->reply(ERR_NOTONCHANNEL(channel_name));
			return ;
		}

		//Find First User in Channel Operator List
		if (foundChannel->is_User_Operator(user) == 0)
		{
			std::cout << "user is not operator\n";
			//reply("to First User that he cannot Kick second User");
			user->reply(ERR_CHANOPRIVSNEEDED(user->getNick(), channel_name));
			return ;
		}
		//  Find Second User by Nick in Channel and return a pointer to Second User Object
		if (foundChannel->isUserinChannelbys(target) == NULL)
		{
			return ;
		}
		User* secondUser = foundChannel->isUserinChannelbys(target);

		// We know the Channel Pointer, Pointer for the First User and Pointer for Second User
		std::string toSend = ":" + user->_server->_serverName + " KICK " + channel_name + " " + secondUser->getNick() + " :No Reason";
		this->replyLightAll(toSend.c_str(), foundChannel);

		foundChannel->deleteUser(target); // instead of deleteUser => use killUser()
		secondUser->delete_Channel(foundChannel);
}

/*        BAN        */
// format: BAN <channel> <user> [<comment>] 
 void User::execute_ban_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	(void) cmd_name;
	if (args.size() < 2 )
	{
		return;
	}

	const std::string channel_name = args[0];
	std::string target = args[1];
	std::string reason = "";

	// Find Channel in Server Channel List and return it's Channel Obj Pointer
	if (user->_server->findChannel(channel_name) == NULL)
	{
		user->reply(ERR_NOSUCHCHANNEL(user->getNick(), channel_name));
		return ;
	}
	Channel* foundChannel = user->_server->findChannel(channel_name);
	
	// Find if user that wants to kick someone is in same channel 
	// Find First User in Channel::UsersinChannelList or in User::_channelList
	// Compare channels
	// We know have Channel Pointer and First User Pointer

	//  Find First User by User Obj Pointer in Channel::UserList and return 1 if True, 0 if False
	if (foundChannel->isUserinChannelbyp(user) == NULL)
	{
		this->reply(ERR_NOTONCHANNEL(channel_name));
		return ;
	}

	//  Find Second User by Nick in Channel and return a pointer to Second User Object
	if (foundChannel->isUserinChannelbys(target) == NULL)
	{
		user->reply(ERR_USERNOTINCHANNEL(target, channel_name));
		return ;
	}

	User* secondUser = foundChannel->isUserinChannelbys(target);
	// We know the Channel Pointer, Pointer for the First User and Pointer for Second User
	// Find First User in Channel Operator List
	if (foundChannel->is_User_Operator(user) == 0)
	{
		user->reply(ERR_CHANOPRIVSNEEDED(user->getNick(), channel_name));
		return ;
	}
	
	std::string toSend = ":" + user->_server->_serverName + " KICK " + channel_name + " " + secondUser->getNick() + " :No Reason";
	this->replyLightAll(toSend.c_str(), foundChannel);

	foundChannel->deleteUser(target); // removed user from server
	secondUser->delete_Channel(foundChannel);

	foundChannel->addBanned(secondUser);
	foundChannel->deleteUser(target);
	secondUser->delete_Channel(foundChannel);

	// Notify All Users in Channel that User2 has been Banned
	std::cout << target << " has been Banned and Kicked from channel " << channel_name << ".\n";
}

/*        JOIN        */
void User::execute_join_cmd(User* user, std::string& cmd_name, std::vector<std::string> args)
{
	(void) user;
	(void) cmd_name;
	if (this->isRegistered() == false) {
		this->reply(ERR_NOTREGISTERED(this->getNick()));
		return ;
	}
	if (args.size() != 1)
		{
			replyLight("join :only one channel name allowed as parameter");
			return ;
		}
	if (args[0].empty())
		{
			replyLight("join :no channel name detected");
			return ;
		}
	if(this->_server->findChannel(args[0]) == NULL)
	{
		if (args[0].at(0) != '#')
		{
			replyLight("Wrong input for channel name, try with #");
			return ;
		}

		Channel* newChannel = new Channel(args[0]);
		this->_server->addChannel(newChannel);
		newChannel->_server = this->_server;
		newChannel->addCreator(this);
		newChannel->addUser(this);
		this->reply(RPL_JOIN(this->getNick(), newChannel->getName()));
		this->_channelList.push_back(newChannel);

		//this part is for making client recognize operators: / has to be sent on login
		std::string allUserList = newChannel->findAllUsers();
		std::string reply_construct = ":" + this->_server->_serverName + " 353 " + this->getNick() + " = " + newChannel->getName() + " :" + allUserList;
		this->replyLight(reply_construct.c_str());
		std::string reply_endOfNamesList = ":" + this->_server->_serverName + " 366 " + this->getNick() + " " + newChannel->getName() + " :End of /NAMES list.";
		this->replyLight(reply_endOfNamesList.c_str());
		std::string newCNotice = "New channel " + newChannel->getName() + " created by you. You are operator.";
		this->replyLight(newCNotice.c_str());

		newChannel->_countClients++;
	}
	else 
	{
		Channel* foundChannel = this->_server->findChannel(args[0]);
		if (foundChannel->ifBanned(this) == true)
			this->reply(ERR_BANNEDFROMCHAN(args[0]));
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
			std::string joinReply = ":" + this->getPrefix() + " JOIN " + foundChannel->getName();
			this->replyLightAll(joinReply.c_str(), foundChannel); //sending the join rpl to all users in channel
			std::string allUserList = foundChannel->findAllUsers();
			std::string reply_construct = ":" + this->_server->_serverName + " 353 " + this->getNick() + " = " + foundChannel->getName() + " :" + allUserList;
			this->replyLightAll(reply_construct.c_str(), foundChannel);
			std::string newCNotice = "Joined channel " + foundChannel->getName() + ". You are not an operator.";
			this->replyLight(newCNotice.c_str());
			foundChannel->_countClients++;
		}
	}
}


// check if args
// the reply

// PRIVMSG <receiver> <text to be sent>
void User::execute_privmsg_cmd(std::string cmd_name, User* user, std::vector<std::string> args)
{
	std::cout << "Reached execute_privmsg_cmd: Start" << std::endl;

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

	if (receiver[0] == '#') // #: send to all in channel, here: check if channel exists
	{
		std::string channel_name = receiver.substr(0);
		Channel* channel = user->get_channel_if_in(channel_name); // is calling user in that channel he wants to send to?
		if (channel == NULL)
		{
			user->reply(ERR_NOSUCHCHANNEL(user->getNick(), channel_name));
			return ;
		}
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
	// actually sending the priv msg (can make one write/send function out of it for other uses)
	std::string priv_msg = RPL_PRIVMSG(user->getPrefix(), receiver, msg);
	std::string msg_buf = priv_msg + "\r\n";
	if (send(target_user->_fd, msg_buf.c_str(), msg_buf.length(), 0) < 0)
		std::cerr << "Error while sending message to client." << std::endl;
	if (send(this->_fd, msg_buf.c_str(), msg_buf.length(), 0) < 0)
		std::cerr << "Error while sending message to client." << std::endl;
}

// NOTICE <receiver> <text to be sent>
void User::execute_notice_cmd(std::string cmd_name, User* user, std::vector<std::string> args)
{
	std::cout << "Reached execute_notice_cmd: Start" << std::endl;

	if (args[0].empty() || args.size() < 2 || args[1].empty())
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), "NOTICE"));
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

	if (receiver[0] == '#') // #: send to all in channel, here: check if channel exists
	{
		std::string channel_name = receiver.substr(0);
		Channel* channel = user->get_channel_if_in(channel_name); // is calling user in that channel he wants to send to?
		if (channel == NULL)
		{
			user->reply(ERR_NOSUCHCHANNEL(user->getNick(), channel_name));
			return ;
		}
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
	// actually sending the priv msg (can make one write/send function out of it for other uses)
	std::string priv_msg = RPL_PRIVMSG(user->getPrefix(), receiver, msg);
	std::string msg_buf = priv_msg + "\r\n";
	if (send(target_user->_fd, msg_buf.c_str(), msg_buf.length(), 0) < 0)
		std::cerr << "Error while sending message to client." << std::endl;
}


/* 			NICK			*/
void User::execute_nick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	(void) cmd_name;
	std::cout << "Reached execute Nick 1" << std::endl;
	if (args.size() == 0)
	{
		user->reply(ERR_NONICKNAMEGIVEN(user->getNick()));
		return ;
	}
	std::string nickname = args[0];
	std::cout << "Reached execute Nick 2" << std::endl;
	if (user->_server->findByNick(nickname) != NULL)
	{
		//std::cout << "Reached execute Nick 3" << std::endl;
		user->reply(ERR_NICKNAMEINUSE((user->getNick())));
		return ;
	}
	std::cout << "Reached execute Nick 3.5" << std::endl;
	if (this->getNick() == "")
	{
		std::cout << "Reached execute Nick 4" << std::endl;
		user->setNick(nickname);
		user->reply(RPL_WELCOME(nickname));
		user->replyLight("\nYou can use following commands on server:\n  /pass <server_password>\n  /join #<channelname>\n  /nick <newnickname>\n\nand following commands in a channel:\n  /part\n  /quit\n  /privmsg <nickname>\n\nand following commands as an operator:\n  /mode -o OR mode +o <nickname>\n  /kick <nickname>\n");
		return ;
	}
	std::cout << "Reached execute Nick 5" << std::endl;
	std::string toSend = ":" + user->getNick() + " NICK " + nickname;
	user->replyLight(toSend.c_str());
	user->setNick(nickname);
	user->setUsername(nickname);
	std::cout << "Reached execute Nick 6" << std::endl;
	//Notify all Users in All channels in which the User is in about the nicknamechange
	std::vector<Channel*>::iterator itr;
	for (itr = this->_channelList.begin(); itr != this->_channelList.end(); itr++) {
		Channel* foundChannel = (*itr);
		this->replyLightAll(toSend.c_str(), foundChannel);
	}
}

// Use of the extended prefix (['!' <user> ] ['@' <host> ]) must
// not be used in server to server communications and is only
// intended for server to client messages in order to provide
// clients with more useful information about who a message is
// from without the need for additional queries.
// -> calling start in server leading to here.
std::string User::getPrefix()
{
	char* buf = this->_host;
	std::string hostname;
	hostname.assign(buf);
	std::string return_msg = this->getNick();

	/* :<nickname>!<username>@host CMD <target> \r\n */
	if (this->getNick().empty() == false) {
		return_msg += "!" + this->getNick();
		if (hostname.empty() == false)
			return_msg += "@" + hostname;
	}
	return(return_msg);
 }

/*
*  The MODE command is a dual-purpose command in IRC.  It allows both
   usernames and channels to have their mode changed.  The rationale for
   this choice is that one day nicknames will be obsolete and the
   equivalent property will be the channel.
* format: MODE <channelname> {[+|-]o} [<user>] 
* o - give/take channel operator privileges;
*/
void	User::execute_mode_cmd(std::string cmd_name, std::vector<std::string> cmd_args)
{
	// parsing
	if (cmd_args.size() != 2 && cmd_args.size() != 1 && cmd_args.size() != 3)
	// if (cmd_args.size() != 3)
	{
		this->reply(ERR_NEEDMOREPARAMS(this->getNick(), cmd_name));
		return;
	}
	if (cmd_args[0].at(0) != '#')
	{
		//472     ERR_UNKNOWNMODE

		//std::cerr << "format: /mode +o nickname" << std::endl;
		replyLight("mode :wrong format, use: /mode +/-o <nickname> while inside a channel");
		return;
	}
	// getting parameters
	std::string mode;
	const std::string channel_name = cmd_args[0]; // with or without # ?
	if (cmd_args.size() > 1) {
		mode = cmd_args[1];
		std::cout << mode << std::endl;
	}
	std::string target;
	if (cmd_args.size() > 2) {
		target = cmd_args[2];
		std::cout << target << std::endl;
	}
	// check if mode exists: only +/- o implemented
	if (!mode.empty()) {
		if (mode.at(1) != 'o' && (mode.at(0) != '+' || mode.at(0) != '-'))
	{
		this->reply(ERR_UNKOWNMODE(mode));
		return;
	} }
	// check if channel exists
	Channel *temp = this->_server->findChannel(channel_name);
	if (temp == NULL)
	{
		this->reply(ERR_NOSUCHCHANNEL(this->getNick(), channel_name));
		return;
	}
	// check if user is on channel
	Channel* channel = this->get_channel_if_in(channel_name);
	if (channel == NULL)
	{
		this->reply(ERR_NOTONCHANNEL(channel_name));
		return;
	}
	// check if executing user is admin
	if (channel->ifOperator(this->getNick()) == 0)
	{
		this->reply(ERR_CHANOPRIVSNEEDED(this->getNick(), channel_name));
		return;	
	}
	// actual changing of the mode
	User* target_user = channel->isUserinChannelbys(target);
	if (mode.at(0) == '+')
	{
		// nickname!username@servername MODE #channelname +o :targetnick
		channel->setOperator(target_user);
		target_user->reply(RPL_YOUREOPER(target_user->getNick(), channel->getName()));

		std::string reply_construct = ":" + this->getNick() + "!" + this->getUsername() + "@" + this->_server->_serverName + " MODE " + channel->getName() + " +o :" + target_user->getNick();
		this->replyLightAll(reply_construct.c_str(), channel);
		return;
	}
	else if (mode.at(0) == '-')
	{
		channel->deleteOperator(target_user);
		std::string userNameOp = this->getNick() + " is not an operator anymore";
		target_user->replyLightAll(userNameOp.c_str(), channel);
		std::string allUserList = channel->findAllUsers();
		std::string reply_construct = ":" + this->getNick() + "!" + this->getUsername() + "@" + this->_server->_serverName + " MODE " + channel->getName() + " -o :" + target_user->getNick();
		this->replyLightAll(reply_construct.c_str(), channel);
		return;
	}
}


// format: QUIT (:)[<quitmsg>] OK NEED TESTING
/* A client session is ended with a quit message.  The server must close
   the connection to a client which sends a QUIT message. If a "Quit
   Message" is given, this will be sent instead of the default message,
   the nickname.
   If, for some other reason, a client connection is closed without  the
   client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs
   on socket), the server is required to fill in the quit  message  with
   some sort  of  message  reflecting the nature of the event which
   caused it to happen.
*/
void User::execute_quit_cmd(const std::string& cmd_name, std::vector<std::string> args)
{
	(void) cmd_name;
	// send quit msg
	std::vector<Channel *> channel = getChannels();
	for(long unsigned int i = 0;  i < getChannels().size(); i++)
	{
		if (args.size() == 0)
		{
			std::string reason = "no reason";
			channel[i]->notify_others("QUIT", reason, this);
		}
		else if (args.size() > 0)
		{
			std::string reason = args[0];
			if (reason.at(0) == ':')
			{
				reason = reason.substr(1);
			}
			channel[i]->notify_others("QUIT", reason, this);
		}
	}
	this->_server->killUser(this); //deletes User from all Channels
}

// format: PING <server1> [<server2>] 
// 		the User pings the server1, if there would be a server2 specified, 
//		server1 would need to forward the Ping command to server2
void User::execute_ping_cmd(const std::string& cmd_name, std::vector<std::string> args)
{
	(void) cmd_name;
	this->reply(("PONG " + args[0]).c_str()); // server replies to client PONG 
}

// format: PASS <password>
void User::execute_pass_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.size() == 0)
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return ;
	}
	if (args.size() > 1)
	{
		user->replyLight("pass :too many parameters");
		return ;
	}
	if (user->isRegistered() == true)
	{
		user->reply(ERR_ALREADYREGISTRED(user->getNick()));
		return ;
	}
	std::string _newPassword = args[0];
	if (user->_server->getPass() == _newPassword)
	{
		this->setRegistered();
		user->replyLight("pass :successfully registered");
		return ;
	}
	else
		user->replyLight("pass :wrong password");
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
	//std::cout << "Reached in the execute_user_cmd Function: Start" << std::endl;
	if (args.size() == 0)
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getUsername(), "User"));
		return ;
	}
	std::string username = args[0];
	//std::cout << "Reached in the execute_user_cmd Function: args[0]: " << args[0] << std::endl;

	user->setUsername(username);
	//std::cout << "Reached in the execute_user_cmd Function: if SetUser: " << getUsername() << "\n" << std::endl;
	std::cout << getUsername();
	std::string toSend = "USER Changed: " + username;
		// std::string toSend = ":" + user->getUsername() + " USER " + username;
	// user->replyLight(toSend.c_str());
	// user->setNick(nickname);
	//Notify all Users in All channels in which the User is in about the username
	std::vector<Channel*>::iterator itr;
	for (itr = this->_channelList.begin(); itr != this->_channelList.end(); itr++) {
		Channel* foundChannel = (*itr);
		this->replyLightAll(toSend.c_str(), foundChannel);
	}
	
	std::cout << "The username is set to: " << this->getUsername() << std::endl;
}


/* format: PART <channel> *( "," <channel> ) [ <Part Message> ]
	 ***********************************************************
	The PART command causes the user sending the message to be removed
   from the list of active members for all given channels listed in the
   parameter string.  If a "Part Message" is given, this will be sent
   instead of the default message, the nickname.  This request is always
   granted by the server.
*/
void User::execute_part_cmd(User* user, std::vector<std::string> args)
{
	std::string part_msg;

	// creating the right parting msg
	// if no msg is given as parameter, the nickname is used for it
		if (args.size() != 2)
		{
			user->replyLight("No params allowed for PART");
			return ;
		}
		// if one msg is given, it is used instead of nickname
		else if (args.size() == 2)
		{
			part_msg = args[1];
		}
		else
		{
			replyLight("Please use format: '/part' without parameters");

			return;
		}
		if (this->_server->findChannel(args[0]) == NULL) {
			replyLight("part :Channel not found");
			return;
		}
		Channel* foundChannel = this->_server->findChannel(args[0]);
		if (foundChannel != NULL)
		{
			if (this->get_channel_if_in(foundChannel->getName()) != NULL)
			{
				// notifying others users about the parting
				// :mynick!mynick@myhost.com PART #mychannel :Leaving
				std::string reply_construct = ":" + this->getNick() + "!" + this->getUsername() + "@" + this->_server->_serverName + " PART " + foundChannel->getName() + " :Leaving";
				this->replyLightAll(reply_construct.c_str(), this->get_channel_if_in(foundChannel->getName()));

				//erasing User from the channellist and operator list
				foundChannel->partUser(getNick());

				// delete Channel from the Users Channellist
				this->delete_Channel(foundChannel);
			}
			else
			{
				this->reply(ERR_NOTONCHANNEL(foundChannel->getName()));
				return;
			}
		}
		else
		{
			this->reply(ERR_NOSUCHCHANNEL(this->getNick(), foundChannel->getName()));
			return;
		}
	}