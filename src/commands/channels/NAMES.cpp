#include <commands.hpp>

#include <Request.hpp>

static std::string	formatted_name(Client *user, Channel *ch)
{
	std::string	str;

	if (user->isChannelOperator(ch))
		str += "@";
	str += "+" + user->getNickName();
	return str;
}

static void	list_channel(Client *sender, Channel *ch)
{
	std::string			response;
	std::set<Client *>	user_list;
	Channel::t_cl_it	it;

	if (!ch)
		return ;
	user_list = ch->getUserList();
	it = user_list.begin();
	if (it == user_list.end())
		return ;
	response += formatted_name(*(it++), ch);
	while (it != user_list.end())
	{
		if (it != user_list.begin())
			response += " ";
		response += formatted_name(*(it++), ch);
	}(void)sender;
	sender->sendReply(RPL_NAMREPLY, "= " + ch->getName() + " :" + response);
	sender->sendReply(RPL_ENDOFNAMES, ch->getName() + " :End of /NAMES list");
}

static void	list_all(std::set<Channel *> ch_list, Client *sender)
{
	std::set<Channel *>::iterator	it;

	it = ch_list.begin();
	while (it != ch_list.end())
		list_channel(sender, *(it++));
}

void	com_Names(t_param_list params, Client *sender, Server *server)
{
	t_param_list		ch_names;
	unsigned int		i;
	Channel				*ch;
	std::set<Channel *>	ch_list;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (!params.empty())
	{
		ch_names = Request().param_split(params.at(0));
		i = 0;
		while (i < ch_names.size())
		{
			ch = server->getChannel(ch_names.at(i++));
			if (ch)
				ch_list.insert(ch);
		}
	}
	if (ch_list.empty())
		ch_list = server->getChannelList();
	list_all(ch_list, sender);
}
