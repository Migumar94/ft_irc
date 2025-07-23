#include <commands.hpp>

void	mode_i_option(std::string options, bool setting, Channel *channel, Client *sender)
{
	std::string	message;

	if (options.find('i') != options.npos)
	{
		channel->getMode()->setInviteOnly(setting);
		message = ":" + sender->getNickName() + " MODE " + channel->getName();
		if (setting)
			message += " +i";
		else
			message += "-i";
		channel->sendText(NULL, message);
	}
}
