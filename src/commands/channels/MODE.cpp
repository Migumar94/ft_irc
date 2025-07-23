#include <commands.hpp>
#include <Request.hpp>

void	channel_modes(t_param_list params, Client *sender, Server *server);
void	user_modes(t_param_list params, Client *sender, Server *server);

void	com_Mode(t_param_list params, Client *sender, Server *server)
{
	t_param_list	channels;
	std::string		target;

	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	if (params.empty())
		return sender->sendReply(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
	target = params.at(0);
	if (Request().isChannel(target))
		return channel_modes(params, sender, server);
	else
		return user_modes(params, sender, server);
}