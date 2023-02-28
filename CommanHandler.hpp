#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "user.hpp"

class User;
class Message;
class Server;

class CommanHandler
{
	public:
		CommanHandler();
		CommanHandler(const CommanHandler& obj);
		CommanHandler& operator=(const CommanHandler& obj);
		~CommanHandler();

		void start(User* user, const std::string& message);

};

#endif