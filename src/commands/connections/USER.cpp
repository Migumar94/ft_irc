#include <commands.hpp>

void	com_User(t_param_list params, Client *sender, Server *server)
{
	std::string	welcome_msg;

	if (!sender->getPasswordMatched())
		return ;
	if (!sender->getUserName().empty())
		return sender->sendReply(ERR_ALREADYREGISTRED, ":You may not reregister");
	if (params.size() < 4)
		return sender->sendReply(ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
	sender->setUserName("~" + params.at(0));
	sender->setRealName(params.at(3));
	if (sender->getNickName().empty())
		return ;
	sender->setRegistered();
	welcome_msg = server->getConfig().getWelcomeMsg();
	if (!welcome_msg.empty())
		sender->sendReply("001", ":" + server->getConfig().getWelcomeMsg());
}
