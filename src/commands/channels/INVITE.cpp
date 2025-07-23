#include <commands.hpp>

void	com_Invite(t_param_list params, Client *sender, Server *server)
{
	std::string	channel_name;
	Channel		*channel;
	std::string	nickname;
	Client		*client;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.size() < 2)
		return sender->sendReply(ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters");
	channel_name = params.at(1);
	channel = server->getChannel(channel_name);
	if (!channel || !channel->hasUser(sender))
		return sender->sendReply(ERR_NOTONCHANNEL, channel_name + " :You're not on that channel");
	if (!sender->isChannelOperator(channel) && !sender->isOperator())
		return sender->sendReply(ERR_CHANOPRIVSNEEDED, channel->getName() + " :You're not channel operator");
	nickname = params.at(0);
	client = server->getClient(nickname);
	if (!client)
		return sender->sendReply(ERR_NOSUCHNICK, nickname + " :No such nick/channel");
	if (channel->hasUser(client))
		return sender->sendReply(ERR_USERONCHANNEL, client->getNickName() + " " + channel->getName() + " :is already on channel");
	if (client->isAway())
		sender->sendReply(RPL_AWAY, client->getNickName() + " :" + client->getAwayMessage());
	channel->getMode()->addToInviteList(nickname);
	sender->sendReply(RPL_INVITING, nickname + " " + channel_name);
	client->sendText(":" + sender->getNickName() + " INVITE " + nickname + " :" + channel_name);
}