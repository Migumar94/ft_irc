#include <commands.hpp>

static void	part_single(std::string channel_name, Client *sender, Server *server, std::string message)
{
	Channel				*channel;
	std::set<Client *>	user_list;

	if (channel_name.empty())
		return ;
	channel = server->getChannel(channel_name);
	if (!channel)
		return sender->sendReply(ERR_NOSUCHCHANNEL, channel_name + " :No such channel");
	else if (!channel->hasUser(sender))
		return sender->sendReply(ERR_NOTONCHANNEL, channel_name + " :You're not on that channel");
	user_list = channel->getUserList();
	if (!message.empty())
		message = " :" + message;
	for (std::set<Client *>::iterator it = user_list.begin(); it != user_list.end(); it++)
		(*it)->sendText(":" + sender->getNickName() + " PART " + channel_name + message);
	channel->removeUser(sender);
	sender->removeChannel(channel);
}

void	com_Part(t_param_list params, Client *sender, Server *server)
{
	t_param_list	channels;
	std::string		message;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.empty())
		return sender->sendReply(ERR_NEEDMOREPARAMS, "PART :Not enough parameters");
	channels = Request().param_split(params.at(0));
	if (params.size() > 1)
		message = params.at(1);
	for (t_param_list::iterator it = channels.begin(); it != channels.end(); it++)
		part_single(*it, sender, server, message);
}
