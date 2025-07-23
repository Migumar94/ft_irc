#include <commands.hpp>

void	com_Topic(t_param_list params, Client *sender, Server *server)
{
	Channel	*channel;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.empty())
		return sender->sendReply(ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters");
	channel = server->getChannel(params.at(0));
	if (!channel)
		return sender->sendReply(ERR_NOSUCHCHANNEL, channel->getName() + " :No such channel");
	if (params.size() < 2)
	{
		if (channel->getTopic().empty())
			sender->sendReply(RPL_NOTOPIC, channel->getName() + " :No topic is set");
		else
			sender->sendReply(RPL_TOPIC, channel->getName() + " :" + channel->getTopic());
		return ;
	}
	if (!channel->hasUser(sender))
		return sender->sendReply(ERR_NOTONCHANNEL, channel->getName() + " :You're not on that channel");
	ChannelMode	*mode = channel->getMode();
	if (mode->isTopicRestricted() && !sender->isChannelOperator(channel) && !sender->isOperator())
		return sender->sendReply(ERR_CHANOPRIVSNEEDED, channel->getName() + " :You're not channel operator");
	channel->setTopic(params.at(1));
	sender->sendReply(RPL_TOPIC, channel->getName() + " :" + channel->getTopic());
}