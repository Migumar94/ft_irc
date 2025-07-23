#include <commands.hpp>

int	mode_o_option(std::string options, bool setting, t_param_list::iterator &arg, t_param_list &params, Channel *channel, Client *sender)
{
	ChannelMode	*mode;
	std::string	message;
	std::string	op_name;

	if (options.find('o') != options.npos)
	{
		mode = channel->getMode();
		while (arg != params.end())
		{
			op_name = *(arg++);
			if (op_name.find(' ') != op_name.npos)
				return 1;
			if (setting)
				mode->addOperator(op_name);
			else
				mode->removeOperator(op_name);
			message = ":" + sender->getNickName() + " MODE " + channel->getName();
			if (setting)
				message += " +o ";
			else
				message += " -o ";
			message += op_name;
			channel->sendText(NULL, message);
		}
	}
	return 0;
}
