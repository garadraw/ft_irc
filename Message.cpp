# include "Message.hpp"

Message::Message()
{
	this->setPrefix("", "", "");
	this->command = std::string("");
	this->flags = std::vector<std::string>();
	this->params = std::vector<std::string>();
	this->freetext = std::string("");
	std::cout << "Default empty Message constructed." << std::endl;
}

Message::Message(std::string message_raw)
{
	// Might use private parsing function(s)... OR NOT...
	this->setPrefix("", "", "");

	// Parse command
	int	message_index = 0;
	int	command_category = 0;

	if (message_raw.length() > 4 && message_raw.at(message_index) == '/' && message_raw.at(message_index + 1) != ' ')
	{
		std::string	cmd_string;

		while (message_raw.at(message_index) != ' ' && message_raw.at(message_index) != '\t')
		{
			cmd_string.push_back(message_raw.at(message_index));
			message_index++;
		}

		for (int i = 0; i < cmd_string.length(); i++)
			this->command.push_back(std::toupper(cmd_string.at(i)));

		// Searching through the first category of commands
		for (std::vector<std::string>::const_iterator it = this->known_commands_one.begin(); it != this->known_commands_one.end(); it++)
		{
			if (*it == this->command)
			{
				command_category = 1;
				break ;
			}
		}
		// if (command_category == 0)
		// ... further for-loops through other vectors of known commands ...

		switch (command_category)
		{
			case 1: // Specifically for NICK, JOIN, PING or QUIT! Commands with only one parameter in the form of a single string.
			{
				std::string	param_string;

				while (message_raw.at(message_index) == ' ' || message_raw.at(message_index) == '\t')
					message_index++;
				while (message_raw.at(message_index) != ' ' && message_raw.at(message_index) != '\t')
				{
					param_string.push_back(message_raw.at(message_index));
					message_index++;
				}
				this->params.push_back(param_string);
			}
			default:
				break ;
		}

		// Parse flags
		// while (message_raw.at(message_index) == ' ' || message_raw.at(message_index) == '\t')
		// 	message_index++;
	}
	else // At this point, the message is identified as being just plain freetext.
	{
		this->command = std::string("");
		this->flags = std::vector<std::string>();
		this->params = std::vector<std::string>();
		this->freetext = message_raw;
	}
}

Message::Message(const Message &original)
{
	this->setPrefix(original.getPrefix().nick, original.getPrefix().user, original.getPrefix().server);
	this->command = original.getCommand();
	this->flags = original.getFlags();
	this->params = original.getParams();
	this->freetext = original.getFreetext();
	std::cout << "Copied Message constructed." << std::endl;
}

Message::~Message()
{
	std::cout << "Message destroyed." << std::endl;
}

Message	&Message::operator=(const Message &original)
{
	this->setPrefix(original.getPrefix().nick, original.getPrefix().user, original.getPrefix().server);
	this->command = original.getCommand();
	this->flags = original.getFlags();
	this->params = original.getParams();
	this->freetext = original.getFreetext();
	std::cout << "Copied Message constructed." << std::endl;
	return (*this);
}

t_prefix	Message::getPrefix(void) const
{
	return (this->prefix);
}

std::string	Message::getCommand(void) const
{
	return (this->command);
}

std::vector<std::string>	Message::getFlags(void) const
{
	return (this->flags);
}

std::vector<std::string>	Message::getParams(void) const
{
	return (this->params);
}

std::string	Message::getFreetext(void) const
{
	return (this->freetext);
}

void	Message::setPrefix(std::string newNick, std::string newUser, std::string newServer)
{
	this->prefix.nick = newNick;
	this->prefix.user = newUser;
	this->prefix.server = newServer;
}

void	Message::setCommand(std::string newCommand)
{
	this->command = newCommand;
}

void	Message::setFlags(std::vector<std::string> newFlags)
{
	for (std::vector<std::string>::iterator it = newFlags.begin(); it != newFlags.end(); it++)
		this->flags.push_back(*it);
}

void	Message::setParams(std::vector<std::string> newParams)
{
	for (std::vector<std::string>::iterator it = newParams.begin(); it != newParams.end(); it++)
		this->params.push_back(*it);
}

void	Message::setFreetext(std::string newFreetext)
{
	this->freetext = newFreetext;
}
