#include <commands.hpp>

void	com_Kick(t_param_list params, Client *sender, Server *server)
{
	Channel		*ch;
	Client		*cl;
	std::string	message;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.size() < 2)
		return sender->sendReply(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters");
	ch = server->getChannel(params.at(0));
	if (!ch)
		return sender->sendReply(ERR_NOSUCHCHANNEL, ch->getName() + " :No such channel");
	if (!ch->hasUser(sender))
		return sender->sendReply(ERR_NOTONCHANNEL, ch->getName() + " :You're not on that channel");
	if (!sender->isChannelOperator(ch) && !sender->isOperator())
		return sender->sendReply(ERR_CHANOPRIVSNEEDED, ch->getName() + " :You're not channel operator");
	cl = server->getClient(params.at(1));
	if (!cl)
		return ;
	if (params.size() > 2)
		message = params.at(2);
	if (!message.empty())
		message = " :" + message;
	ch->sendText(NULL, sender->getNickName() + " KICK " + ch->getName() + " " + cl->getNickName() + message);
	ch->removeUser(cl);
}
