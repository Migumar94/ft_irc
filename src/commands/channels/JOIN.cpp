#include <commands.hpp>
#include <Request.hpp>

static void	single_join(std::string channel_name, std::string password, Client *sender, Server *server);

static void	join_new(std::string channel_name, std::string password, Client *sender, Server *server)
{
	Channel		*ch;

	if (!Request().isChannel(channel_name))
		return sender->sendReply(ERR_NOSUCHCHANNEL, channel_name + " :Channel name not valid");
	ch = server->createChannel(channel_name);
	if (!ch)
		return sender->sendReply(ERR_NOSUCHCHANNEL, channel_name + " :No such channel");
	if (server->getConfig().getMaxChannels() && server->getChannelList().size() >= server->getConfig().getMaxChannels())
		return sender->sendReply(ERR_TOOMANYCHANNELS, channel_name + " :Server do not allow more channels");
	ch->getMode()->addOperator(sender->getNickName());
	single_join(channel_name, password, sender, server);
	if (!ch->hasUser(sender))
		server->	deleteChannel(ch);
}

static void	sendStatusToOthers(Client *sender, Channel *ch, Server *server)
{
	std::set<Client *>	user_list = ch->getUserList();

	for (std::set<Client *>::iterator it = user_list.begin(); it != user_list.end(); it++)
	{
		if (*it != sender)
		{
			if (sender->isChannelOperator(ch))
				(*it)->sendText(":" + server->getConfig().getServerName() + " MODE " + ch->getName() + " +o " + sender->getNickName());
			else
				(*it)->sendText(":" + server->getConfig().getServerName() + " MODE " + ch->getName() + " +v " + sender->getNickName());
		}
	}
}

static void	single_join(std::string channel_name, std::string password, Client *sender, Server *server)
{
	Channel			*ch;
	t_param_list	param;
	ChannelMode		*mode;

	if (channel_name.empty())
		return ;
	ch = server->getChannel(channel_name);
	if (!ch)
		return join_new(channel_name, password, sender, server);
	if (ch->hasUser(sender))
		return ;
	mode = ch->getMode();
	if (mode->isInviteOnly() && !mode->inInviteList(sender->getNickName()) && !sender->isOperator())
		return sender->sendReply(ERR_INVITEONLYCHAN, channel_name + " :Cannot join channel (+i)");
	if (!mode->passwordMatch(password) && !sender->isOperator())
		return sender->sendReply(ERR_BADCHANNELKEY, channel_name + " :Cannot join channel (+k)");
	if (server->getConfig().getMaxChannelsPerUser() &&  sender->getActiveChannels().size() >= server->getConfig().getMaxChannelsPerUser() && !sender->isOperator())
		return sender->sendReply(ERR_TOOMANYCHANNELS, channel_name + " :You have joined too many channels");
	if (mode->getMaxUsers() && ch->getUserList().size() >= mode->getMaxUsers() && !sender->isOperator())
		return sender->sendReply(ERR_CHANNELISFULL, channel_name + " :Cannot join channel (+l)");
	if (mode->isInviteOnly())
		mode->removeFromInviteList(sender->getNickName());
	sender->addChannel(ch);
	ch->addUser(sender);
	ch->sendText(NULL, ":" + sender->getNickName() + " JOIN :" + ch->getName());
	sendStatusToOthers(sender, ch, server);
	param.push_back(channel_name);
	com_Names(param, sender, server);
	com_Topic(param, sender, server);
}

static void	multiple_join(t_param_list channels, t_param_list passwords, Client *sender, Server *server)
{
	unsigned int	i;
	std::string		password;

	i = 0;
	while (i < channels.size())
	{
		if (i < passwords.size())
			password = passwords.at(i);
		else
			password = std::string();
		single_join(channels.at(i), password, sender, server);
		i++;
	}
}

void	com_Join(t_param_list params, Client *sender, Server *server)
{
	t_param_list	channels;
	t_param_list	passwords;
	std::string		channel_name;
	std::string		password;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.empty())
		return sender->sendReply(ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
	channels = Request().param_split(params.at(0));
	if (params.size() > 1)
		passwords = Request().param_split(params.at(1));
	if (channels.size() > 1)
		multiple_join(channels, passwords, sender, server);
	else
	{
		channel_name = channels.at(0);
		if (passwords.size())
			password = passwords.at(0);
		single_join(channel_name, password, sender, server);
	}
}
