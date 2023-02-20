#include "CommandHandler.hpp"

/*
* getline(): gets a line until delimitor;
* message: the message read in by the servers file descriptor
* client needs to be initizalied with the fd of the server
*/
void CommandHandler::start(User* user, const std::string& message) // client = user
{
	std::string line;
	std::stringstream s_stream(message);
	std::string cmd_name;

	while (std::getline(s_stream, line))
	{
		if (line[line.length -1] == '\r') // irc messages are terminated with \r carriage return 
		{
			line = line.substr(0, line[line.length() - 1]);
			cmd_name = line.substr(0, line.find(32)); // 32 == space
			//std::remove_if(cmd_name.begin(), cmd_name.end(), isspace); remove spaces befor/after
		}
		try
		{
			Command *cmd = this->_commandos.at(cmd_name); // where/when filled ?

			std::vector<std::string> cmd_args;

			std::string buffer;
			std::stringstream s_stream_args(line.substr(cmd_name.length(), line.length())); // end of cmd_name till eol

			while (s_stream_args >> buffer) // read args of the cmd in as whole string;
			{
				cmd_args.push_back(buffer);
			}

			if (user->isRegistered() == false && command->authy_needed() == true)
			{
				user->reply(ERR_NOTREGISTERED(user->getNick())); // errormsg from documentation
				return;
			}
			//cmd->execute(user, args);
			if (cmd_name.compare("JOIN") == 0)
			{
				user.execute_join_cmd(user, cmd_name, cmd_args);
			}
			else if (cmd_name.compare("KICK") == 0)
			{
				user.execute_kick_cmd(user, cmd_name, cmd_args);
			}
		}
		catch (const std::out_of_range& e) // thrown by vector or string
		{
			user->reply(ERR_UNKNOWNCOMMAND(cmd_name));
		}
	}
}
