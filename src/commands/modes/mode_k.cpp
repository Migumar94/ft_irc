#include <commands.hpp>

int	get_password(std::string options, bool setting, t_param_list::iterator &arg, t_param_list &params, std::string &new_password)
{
	std::string::iterator	c;

	if (options.find('k') != options.npos)
	{
		if (setting)
		{
			if (arg == params.end() || (*arg).find(' ') != (*arg).npos)
				return 1;
			new_password = *(arg++);
		}
	}
	return 0;
}

void	mode_k_option(std::string options, bool setting, Channel *channel, Client *sender, std::string password)
{
	std::string message;

	if (options.find('k') != options.npos)
	{
		channel->getMode()->setPassword(password);
		message = ":" + sender->getNickName() + " MODE " + channel->getName();
		if (setting)
			message += " +k " + password;
		else
			message += "-k";
		channel->sendText(NULL, message);
	}
}
