#include <commands.hpp>

void	com_Nick(t_param_list params, Client *sender, Server *server)
{
	std::string	nick;
	std::string welcome_msg;

	if (params.empty())
		return sender->sendReply(ERR_NONICKNAMEGIVEN, ":No nickname given");
	if (!sender->getNickName().empty())
		return sender->sendReply(ERR_ALREADYREGISTRED, ":You may not reregister");
	nick = params.at(0);
	if (!Request().isNickname(nick))
		return sender->sendReply(ERR_ERRONEUSNICKNAME, nick + " :Erroneus nickname");
	if (server->getClient(nick))
		return sender->sendReply(ERR_NICKNAMEINUSE, nick + " :Nickname is already in use");
	sender->setNickName(nick);
	if (sender->getUserName().empty())
		return ;
	sender->setRegistered();
	welcome_msg = server->getConfig().getWelcomeMsg();
	if (!welcome_msg.empty())
		sender->sendReply("001", ":" + server->getConfig().getWelcomeMsg());
}
