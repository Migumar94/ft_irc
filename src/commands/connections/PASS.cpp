#include <commands.hpp>

void	com_Pass(t_param_list params, Client *sender, Server *server)
{
	if (sender->getPasswordMatched())
		return sender->sendReply(ERR_ALREADYREGISTRED, ":You may not reregister");
	if (params.empty())
		return sender->sendReply(ERR_NEEDMOREPARAMS, "PASS :Not enough parameters");
	if (!server->passwordMatch(params.at(0)))
	{
		sender->sendReply(ERR_PASSWDMISMATCH, ":Password incorrect");
		server->deleteClient(sender);
		return ;
	}
	sender->setPasswordMatched();
}
