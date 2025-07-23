#include <commands.hpp>

static bool	 is_user_mode(char c)
{
	return (c == 'o');
}

static bool	 valid_options(std::string options, Client *sender)
{
	std::string::iterator	c;

	c = options.begin();
	if (c != options.end() && (*c == '+' || *c == '-'))
		c++;
	while (c != options.end())
	{
		if (!is_user_mode(*c))
		{
			sender->sendReply(ERR_UNKNOWNMODE, *c + " :is unknown mode char to me");
			return false;
		}
		c++;
	}
	return true;
}

static int	edit_user_mode(t_param_list params, Client *sender, Server *server, Client *client)
{
	bool					setting;
	std::string				options;

	if (params.size() < 2)
		return 1;
	options = params.at(1);
	if (!valid_options(options, sender))
		return 1;
	setting = true;
	if (options.at(0) == '-')
		setting = false;
	if (options.find('o') != options.npos)
	{
		if (setting)
			server->addOperator(client->getNickName());
		else
			server->removeOperator(client->getNickName());
	}
	return 0;
}

static void	user_mode_info(Client *user, Client *sender)
{
	std::string		str = " +";

	if (user->isOperator())
		str += "o";
	sender->sendReply(RPL_UMODEIS, user->getNickName() + str);
}

void	user_modes(t_param_list params, Client *sender, Server *server)
{
	Client	 	*client;

	client = server->getClient(params.at(0));
	if (!client)
		return sender->sendReply(ERR_NOSUCHCHANNEL, params.at(0) + " :No such nick/channel");
	if (params.size() == 1)
		return user_mode_info(client, sender);
	if (!sender->isOperator())
		return sender->sendReply(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator");
	if (edit_user_mode(params, sender, server, client))
		return sender->sendReply(ERR_UNKNOWNMODE, ":MODE usage: /mode nickname {+|-}o");
}