#include <commands.hpp>

void	mode_t_option(std::string options, bool setting, Channel *channel, Client *sender)
{
	std::string	message;

	if (options.find('t') != options.npos)
	{
		channel->getMode()->setTopicRestricted(setting);
		message = ":" + sender->getNickName() + " MODE " + channel->getName();
		if (setting)
			message += " +t";
		else
			message += "-t";
		channel->sendText(NULL, message);
	}
}
