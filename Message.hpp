#ifndef MESSAGE_HPP
# define MESSAGE_HPP "Message.hpp"

# include "server.hpp"

typedef struct	s_prefix
{
	std::string	nick;
	std::string	user;
	std::string	server;
}				t_prefix;

class Message // CURRENT STATUS: Can only generate Messages with commands NICK, JOIN, PING and QUIT (NOT TESTED YET)
{
	private:
		const std::vector<std::string>	known_commands{"/PASS", "/QUIT", "/NICK", "/NOTICE", "/USER", "/PART", "/MODE", "/JOIN", "/PRIVMSG", "/PING", "/PONG", "/KICK", "/BAN"};
		const std::vector<std::string>	known_commands_one{"/NICK", "/JOIN", "/PING", "/QUIT"};
		t_prefix						prefix;		// CURRENT STATUS: DOES NOT GET POPULATED
		std::string						command;	// CURRENT STATUS: IMPLEMENTED
		std::vector<std::string>		flags;		// CURRENT STATUS: DOES NOT GET POPULATED
		std::vector<std::string>		params;		// CURRENT STATUS: IMPLEMENTED
		std::string						freetext;	// CURRENT STATUS: DOES NOT GET POPULATED

	public:
		Message();
		Message(std::string message_raw);
		Message(const Message &original);
		~Message();

		Message	&operator=(const Message &original);

		t_prefix					getPrefix(void) const;
		std::string					getCommand(void) const;
		std::vector<std::string>	getFlags(void) const;
		std::vector<std::string>	getParams(void) const;
		std::string					getFreetext(void) const;
		void	setPrefix(std::string newNick, std::string newUser, std::string newServer);
		void	setCommand(std::string newCommand);
		void	setFlags(std::vector<std::string> newFlags);
		void	setParams(std::vector<std::string> newParams);
		void	setFreetext(std::string newFreetext);
};

#endif
