#include <commands.hpp>

void	com_Oper(t_param_list params, Client *sender, Server *server)
{
	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.size() < 2)
		return sender->sendReply(ERR_NEEDMOREPARAMS, ":Not enough parameters");
	if (!server->getOperatorList().validateOper(params.at(0), params.at(1)))
		return sender->sendReply(ERR_PASSWDMISMATCH, ":Password incorrect");
	server->addOperator(sender->getNickName());
	sender->sendReply(RPL_YOUREOPER, ":You are now an IRC operator");
}
