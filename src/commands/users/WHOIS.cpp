#include <commands.hpp>

static void	single_whois(Client *sender, Client *client, Server *server)
{
	std::set<Channel *>	channels;
	std::string			channels_str;

	sender->sendReply(RPL_WHOISUSER, client->getNickName() + " " + client->getUserName() + " " + server->getConfig().getNetworkName() + " " + server->getConfig().getServerName() + " :" + client->getRealName());
	if (client->isOperator())
		sender->sendReply(RPL_WHOISOPERATOR, client->getNickName() + " :is and IRC operator");
	channels = client->getActiveChannels();
	for (std::set<Channel *>::const_iterator it = channels.begin(); it != channels.end(); it++)
	{
		channels_str += " ";
		if (it == channels.begin())
			channels_str += ":";
		channels_str += (*it)->getName();
	}
	sender->sendReply(RPL_WHOISCHANNELS, client->getNickName() + channels_str);
	sender->sendReply(RPL_WHOISSERVER, client->getNickName() + " " + server->getConfig().getServerName());
}

void	com_WhoIs(t_param_list params, Client *sender, Server *server)
{
	Client 			*client;
	t_param_list	nicknames;

	if (params.empty())
		return sender->sendReply(RPL_ENDOFWHOIS, ":End of /WHOIS list");
	nicknames = Request().param_split(params.at(0));
	for (t_param_list::const_iterator it = nicknames.begin(); it != nicknames.end(); it++)
	{
		client = server->getClient(*it);
		if (!client)
		{
			sender->sendReply(ERR_NOSUCHNICK, ":No such nick/channel");
			continue ;
		}
		single_whois(sender, client, server);
	}
	sender->sendReply(RPL_ENDOFWHOIS, ":End of /WHOIS list");
}