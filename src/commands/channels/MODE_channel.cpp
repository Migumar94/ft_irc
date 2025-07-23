#include <commands.hpp>
#include <utils.hpp>
#include <modes.hpp>

static bool	 is_channel_mode(char c)
{
	return (c == 'o' || c == 'i' || c == 't' || c == 'n' || c == 'l' || c == 'k');
}

static bool	 valid_options(std::string options, Client *sender)
{
	std::string::iterator	c;
	std::string				option_str;

	c = options.begin();
	if (c != options.end() && (*c == '+' || *c == '-'))
		c++;
	while (c != options.end())
	{
		if (!is_channel_mode(*c))
		{
			option_str += *c;
			sender->sendReply(ERR_UNKNOWNMODE, option_str + " :is unknown mode char to me");
			return false;
		}
		c++;
	}
	return true;
}

static int	edit_channel_mode(t_param_list params, Client *sender, Server *server, Channel *channel)
{
	bool					setting;
	std::string				options;
	t_param_list::iterator	arg;
	std::string				new_password;
	unsigned int			new_limit;

	if (!params.size())
		return 1;
	arg = params.begin();
	arg++;
	options = *(arg++);
	if (!valid_options(options, sender))
		return 1;
	setting = true;
	if (options.at(0) == '-')
		setting = false;
	if (get_limit(options, setting, arg, params, new_limit))
		return 1;
	if (get_password(options, setting, arg, params, new_password))
		return 1;
	if (mode_o_option(options, setting, arg, params, channel, sender))
		return 1;
	else if (arg != params.end())
		return 1;
	mode_i_option(options, setting, channel, sender);
	mode_n_option(options, setting, channel, sender);
	mode_t_option(options, setting, channel, sender);
	mode_l_option(options, setting, channel, sender, server, new_limit);
	mode_k_option(options, setting, channel, sender, new_password);
	return 0;
}

static void	channel_mode_info(Channel *channel, Client *sender)
{
	std::string		str = " +";
	unsigned int	max_users;
	std::string		password;
	ChannelMode		*mode;

	mode = channel->getMode();

	if (mode->isInviteOnly())
		str += "i";
	password = mode->getPassword();
	if (!password.empty())
		str += "k";
	max_users = mode->getMaxUsers();
	if (max_users)
		str += "l";
	if (mode->isOutsiteForbidden())
		str += "n";
	if (mode->isTopicRestricted())
		str += "t";
	if (max_users)
		str += " " + utostr(max_users);
	if (!password.empty())
		str += " " + password;
	sender->sendReply(RPL_CHANNELMODEIS, channel->getName() + str);
}

void	channel_modes(t_param_list params, Client *sender, Server *server)
{
	Channel 	*channel;

	channel = server->getChannel(params.at(0));
	if (!channel)
		return sender->sendReply(ERR_NOSUCHCHANNEL, channel->getName() + " :No such channel");
	if (params.size() == 1)
		return channel_mode_info(channel, sender);
	if (!sender->isChannelOperator(channel) && !sender->isOperator())
		return sender->sendReply(ERR_CHANOPRIVSNEEDED, channel->getName() + " :You're not channel operator");
	if (edit_channel_mode(params, sender, server, channel))
		return sender->sendReply(ERR_UNKNOWNMODE, ":MODE usage: /mode {+|-}{i|k|l|n|o|t} {limit} {password} {nicknames}");
}