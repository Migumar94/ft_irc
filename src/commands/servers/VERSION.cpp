#include <commands.hpp>

void	com_Version(t_param_list params, Client *sender, Server *server)
{
	char	debuglevel;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (!params.empty())
	{}
	debuglevel = '0';
	#ifdef DEBUG_MODE
	debuglevel = '2';
	#endif
	return sender->sendReply(RPL_VERSION, server->getConfig().getVersion() + "." + debuglevel + " " + server->getConfig().getServerName() + " :eval");
}
