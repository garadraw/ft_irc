#include "server.hpp"

std::vector<std::string> Server::parseIncomingMsg(std::string buffer)
{
    //checking for prefix
    // if prefix, then prefinx is first string in vector, otherwise the first string is null
    //second string in vector is command plus arguments
    std::vector<std::string> parsedMsg;

    if (buffer.empty() 
        || ((buffer[buffer.size()] != '\n') 
        && (buffer[buffer.size() - 1] != '\r')))
        return std::vector<std::string> {};
    if(buffer[0] == ':')
    {
        size_t end = buffer.find(' ');
        parsedMsg.push_back(buffer.substr(1, end - 1));
        parsedMsg.push_back(buffer.substr(end + 1, buffer.size() - end + 1));
    }
    else
    {
        parsedMsg.push_back("");
        parsedMsg.push_back(buffer);
    }
    return (parsedMsg);
}