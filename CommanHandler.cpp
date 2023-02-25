#include "CommanHandler.hpp"

/*
* getline(): gets a line until delimitor;
* message: the message read in by the servers file descriptor
* client needs to be initizalied with the fd of the server
*/
void CommanHandler::start(User* user, const std::string& message) // client = user // testing
{
	std::string line;
	std::stringstream s_stream(message);
	std::string cmd_name;

	// std::cout << "Reached debugging point 3" << std::endl;
	while (std::getline(s_stream, line)) //line: PRIVMSG #test :hei \r\n
	{
		if (line[line.length() -1] == '\r') // irc messages are terminated with \r carriage return 
		{
			//line[4] = line.substr(0, /*line[line.length() - 1]*/ line.find('\r'));
			line = line.substr(0, (line.length() - 1)); //*/ line.find('\r'));
			cmd_name = line.substr(0, line.find(32)); // 32 == space
			std::cout << "cmd_name: " << cmd_name << std::endl;
			//std::remove_if(cmd_name.begin(), cmd_name.end(), isspace); remove spaces befor/after
		}
		try
		{
			std::vector<std::string> cmd_args;

			std::string buffer = line.substr(cmd_name.length(), line.length());
			std::cout << "buffer string contents: " << buffer << std::endl;
			std::stringstream s_stream_args(buffer); // end of cmd_name till eol
			std::string output;

			while (s_stream_args >> output) // automatically splits at " "
			{
				std::cout << "output: " << output << std::endl;
				cmd_args.push_back(output);
			}
			/*
			std::string temp;
			std::string buff = line.substr(cmd_name.length(), line.length());
				while (line[i] != '\r' && line[i] != '\n' && line[i] != '\0')
				{
					if (line[i] == 32)
					{
						temp = line.substr(i, line.find(32));
						cmd_args.push_back(temp);
						temp.clear();
					}
					i++;
				}
			*/

			std::cout << "in handler:cmd_args[0]: " << cmd_args[0] << std::endl;
			std::cout << "in handler:cmd_args[1]: " << cmd_args[1] << std::endl;
			// probably not needed at all:
					// if (user->isRegistered() == false && cmd->authy_needed() == true)
					// {
					// 	user->reply(ERR_NOTREGISTERED(user->getNick())); // errormsg from documentation
					// 	return;
					// }
					// cmd->execute(user, cmd_args);
			
			if (cmd_name.compare("JOIN") == 0) // DONE
			{
				user->execute_join_cmd(user, cmd_name, cmd_args);
				// std::cout << "reached debugging point 4 - JOIN cmd" << std::endl;
			}

			//VALENTIN doing: 
				// else if (cmd_name.compare("KICK") == 0)
				// {
				// 	user->execute_kick_cmd(user, cmd_name, cmd_args);
				// }

			else if (cmd_name.compare("NICK") == 0)
			{
				user->execute_nick_cmd(user, cmd_name, cmd_args);
			}

			else if (cmd_name.compare("PASS") == 0 || cmd_name.compare("pass") == 0) // DONE
			{
				user->execute_pass_cmd(user, cmd_name, cmd_args[0]);
			}

			// else if (cmd_name.compare("NOTICE") == 0)
			// {
			// 	user->execute_notice_cmd(cmd_name, cmd_args);
			// }

			// else if (cmd_name.compare("OPER") == 0)
			// {
			// 	user->execute_oper_cmd(cmd_name, cmd_args);
			// }

			// else if (cmd_name.compare("PART") == 0)
			// {
			// 	user->execute_part_cmd(cmd_name, cmd_args);
			// }

			else if (cmd_name.compare("PRIVMSG") == 0) // DONE
			{
				user->execute_privmsg_cmd(cmd_name, user, cmd_args);
			}
			// else if (cmd_name.compare("MODE") == 0)
			// {
			// 	user->execute_mode_cmd(cmd_name, cmd_args);
			// }

			// else if (cmd_name.compare("KILL") == 0)
			// {
			// 	user->execute_kill_cmd(...);
			// }

			// else if (cmd_name.compare("WHO") == 0)
			// {
			// 	user->execute_who_cmd(cmd_name, cmd_args);
			// }


			// else if (cmd_name.compare("QUIT") == 0)
			// {
			// 	user->execute_quit_cmd(cmd_name, cmd_args);
			// }

			// else if (cmd_name.compare("INVITE") == 0)
			// {
			// 	execute_invite_cmd(user, cmd_name, cmd_args);
			// }

			else if (cmd_name.compare("USER") == 0)
			{
				user->execute_user_cmd(user, cmd_args);
			}
		}
		catch (const std::out_of_range& e) // thrown by vector or string
		{
			// user->reply(ERR_UNKNOWNCOMMAND(cmd_name));
			std::cout << "CommanHandler catch" << std::endl;
		}
	}
}
