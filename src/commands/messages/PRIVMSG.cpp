#include <commands.hpp>

static void	msg_to_user(std::string nickname, std::string message, Client *sender, Server *server)
{
	Client	*client;

	client = server->getClient(nickname);
	if (!client)
		return sender->sendReply(ERR_NOSUCHNICK, nickname + " :No such nick/channel");
	if (client->isAway())
		sender->sendReply(RPL_AWAY, client->getNickName() + " :" + client->getAwayMessage());
	client->sendText(":" + sender->getNickName() + " PRIVMSG " + client->getNickName() + " :" + message);
}

static void	msg_to_channel(std::string channel_name, std::string message, Client *sender, Server *server)
{
	Channel				*ch;
	std::set<Client *>	user_list;
	Channel::t_cl_it	it;
	ChannelMode			*mode;

	ch = server->getChannel(channel_name);
	if (!ch)
		return sender->sendReply(ERR_NOSUCHNICK, channel_name + " :No such nick/channel");
	mode = ch->getMode();
	if (mode->isOutsiteForbidden() && !ch->hasUser(sender))
		return sender->sendReply(ERR_CANNOTSENDTOCHAN, channel_name + " :Cannot send to channel");
	ch->sendText(sender, ":" + sender->getNickName() + " PRIVMSG " + ch->getName() + " :" + message);
}

void	com_PrivMsg(t_param_list params, Client *sender, Server *server)
{
	std::string	target;
	std::string	message;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.empty())
		return sender->sendReply(ERR_NORECIPIENT, ":No recipient given (PRIVMSG)");
	if (params.size() < 2)
		return sender->sendReply(ERR_NOTEXTTOSEND, ":No text to send");
	target = params.at(0);
	message = params.at(1);
	if (server->getBot()->filterMessage(server, sender, message))
		return ;
	if (Request().isChannel(target))
		msg_to_channel(target, message, sender, server);
	else
		msg_to_user(target, message, sender, server);
}
