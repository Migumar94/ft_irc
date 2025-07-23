#include <commands.hpp>
#include <utils.hpp>
#include <stdlib.h>

int	get_limit(std::string options, bool setting, t_param_list::iterator &arg, t_param_list &params, unsigned int &new_limit)
{
	std::string::iterator	c;
	std::string				nbr_string;

	if (options.find('l') != options.npos)
	{
		if (!setting)
			new_limit = 0;
		else if (arg == params.end() || (*arg).empty())
			return 1;
		else
		{
			nbr_string = *arg;
			for (c = nbr_string.begin(); c != nbr_string.end(); c++)
				if (*c < '0' || *c > '9')
					return 1;
			new_limit = atoi(nbr_string.c_str());
			arg++;
		}
	}
	return 0;
}

void	mode_l_option(std::string options, bool setting, Channel *channel, Client *sender, Server *server, unsigned int new_limit)
{
	unsigned int	server_limit;
	std::string		message;

	if (options.find('l') != options.npos)
	{
		server_limit = server->getConfig().getMaxUsersPerChannel();
		if (!new_limit || (server_limit && new_limit > server_limit))
			new_limit = server_limit;
		if (!new_limit)
			setting = false;
		channel->getMode()->setMaxUsers(new_limit);
		message = ":" + sender->getNickName() + " MODE " + channel->getName();
		if (setting)
			message += " +l " + utostr(new_limit);
		else
			message += " -l";
		channel->sendText(NULL, message);
	}
}
