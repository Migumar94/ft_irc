#include <AntiSpamBot.hpp>
#include <commands.hpp>

AntiSpamBot::AntiSpamBot(){}
AntiSpamBot::~AntiSpamBot(){}

bool	AntiSpamBot::hasURL(std::string message)
{
	std::size_t	pos;
	std::size_t	len;
	std::size_t	i;

	len = 0;
	pos = message.find("http://");
	if (pos != message.npos)
		len = 7;
	if (!len)
	{
		pos = message.find("https://");
		if (pos == message.npos)
			return false;
		len = 8;
	}
	message = message.substr(pos + len);
	if (!isalnum(message.at(0)))
		return hasURL(message);
	i = 0;
	while (i < message.size() && isalnum(message.at(i)))
		i++;
	if (i == message.size())
		return false;
	if (message.at(i++) != '.')
		return hasURL(message);
	if (i == message.size())
		return false;
	if (isalnum(message.at(i)))
		return true;
	return hasURL(message);
}

bool	AntiSpamBot::filterMessage(Server *server, Client *sender, std::string message)
{
	std::map<std::string, std::pair<std::string, unsigned int> >::iterator	user;
	std::pair<std::string, std::size_t>										registry;
	t_param_list															kick_message;

	if (hasURL(message))
	{
		sender->sendText(":AntiSpamBot PRIVMSG " + sender->getNickName() + " :Sending URL's is not allowed in this server.");
		return true;
	}
	user = _repetitions.find(sender->getNickName());
	if (user == _repetitions.end())
	{
		registry = std::pair<std::string, std::size_t>(sender->getNickName(), 1);
		_repetitions.insert(std::pair<std::string, std::pair<std::string, std::size_t> >(sender->getNickName(), registry));
		return false;
	}
	if ((*user).second.first != message)
	{
		registry = std::pair<std::string, std::size_t>(message, 1);
		(*user).second = registry;
		return false;
	}
	user->second.second++;
	switch (user->second.second)
	{
		case 2:
		case 3:
			return false;
		case 4:
			sender->sendText(":AntiSpamBot PRIVMSG " + sender->getNickName() + " :Spam is not allowed in this server.");
			sender->sendText(":AntiSpamBot PRIVMSG " + sender->getNickName() + " :Next time you will be kicked.");
			return false;
		default:
			sender->sendText(":AntiSpamBot PRIVMSG " + sender->getNickName() + " :Spam is not allowed in this server.");
			sender->sendText(":AntiSpamBot PRIVMSG " + sender->getNickName() + " :You have been kicked.");
			kick_message.push_back("Spammer kicked from server.");
			com_Quit(kick_message, sender, server);
			return true;
	}
}
