#include <commands.hpp>

static void	single_who(Client *sender, Channel *channel, Server *server)
{
	std::set<Client *>					user_list;
	std::set<Client*>::const_iterator	it;
	Client								*client;
	std::string							str;

	user_list = channel->getUserList();
	for (it = user_list.begin(); it != user_list.end(); it++)
	{
		client = *it;
		if (!client->getUserName().empty() && !client->getNickName().empty())
		{
			str = channel->getName();
			str += " " + client->getUserName();
			str += " localhost";
			str += " " + server->getConfig().getServerName();;
			str += " +" + client->getNickName() + " ";
			if (client->isAway())
				str += "G";
			else
				str += "H";
			if (client->isOperator())
				str += '*';
			if (client->isChannelOperator(channel))
				str += '@';
			str += " :0 " + client->getRealName();
			sender->sendReply(RPL_WHOREPLY, str);
		}
	}
	sender->sendReply(RPL_ENDOFWHO, channel->getName() + " :End of /WHO list");
	t_param_list	channel_name;
	channel_name.push_back(channel->getName());
}

void	com_Who(t_param_list params, Client *sender, Server *server)
{
	std::set<Channel *>					sender_channels;
	std::set<Channel *>::const_iterator	ch_it;
	std::string							channel_name;
	Channel								*channel;
	
	if (!params.empty() && params.at(0) != "0")
	{
		channel_name = params.at(0);
		if (!Request().isChannel(channel_name))
			return sender->sendReply(ERR_NOSUCHCHANNEL, channel_name + " :No such channel");
		channel = server->getChannel(channel_name);
		if (!channel)
			return sender->sendReply(ERR_NOSUCHCHANNEL, channel_name + " :No such channel");
		single_who(sender, channel, server);
	}
	else
	{
		sender_channels = sender->getActiveChannels();
		for (ch_it = sender_channels.begin(); ch_it != sender_channels.end(); ch_it++)
			single_who(sender, *ch_it, server);
	}
}
