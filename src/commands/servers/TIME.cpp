#include <commands.hpp>
#include <ctime>

void	com_Time(t_param_list params, Client *sender, Server *server)
{
	time_t		timestamp;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (!params.empty())
	{}
	time(&timestamp);
	return sender->sendReply(RPL_TIME, ":" + std::string(ctime(&timestamp)));
	(void)server;
}
