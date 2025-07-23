#ifndef ANTI_SPAM_BOT_HPP
#define ANTI_SPAM_BOT_HPP

#include <map>
#include <string>
#include <Server.hpp>

class Server;
class Client;

class AntiSpamBot
{
	private:
		std::map<std::string, std::pair<std::string, unsigned int> >	_repetitions;
		bool	hasURL(std::string message);

	public:
		AntiSpamBot();
		~AntiSpamBot();
		bool	filterMessage(Server *server, Client *sender, std::string message);
};

#endif