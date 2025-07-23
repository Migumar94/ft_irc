#include <commands.hpp>

void	com_Away(t_param_list params, Client *sender, Server *server)
{
	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.empty())
	{
		sender->setAway(false);
		return sender->sendReply(RPL_UNAWAY, ":You are no longer marked as being away");
	}
	sender->setAwayMessage(params.at(0));
	sender->setAway(true);
	return sender->sendReply(RPL_NOWAWAY, ":You have been marked as being away");
	(void)server;
}
