#include <commands.hpp>

void	mode_n_option(std::string options, bool setting, Channel *channel, Client *sender)
{
	std::string	message;

	if (options.find('n') != options.npos)
	{
		channel->getMode()->setOutsideForbidden(setting);
		message = ":" + sender->getNickName() + " MODE " + channel->getName();
		if (setting)
			message += " +n";
		else
			message += "-n";
		channel->sendText(NULL, message);
	}
}
