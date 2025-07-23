#include <commands.hpp>
#include <utils.hpp>
#include <cstdlib>
#include <Request.hpp>

static std::set<Channel *>	get_channel_list(t_param_list params, Server *server)
{
	t_param_list			channels;
	t_param_list::iterator	it;
	std::set<Channel *>		ch_list;
	Channel					*ch;

	if (params.empty())
		return server->getChannelList();
	channels = Request().param_split(params.at(0));
	it = channels.begin();
	while (it != channels.end())
	{
		ch = server->getChannel(*(it++));
		if (ch)
			ch_list.insert(ch);
	}
	return ch_list;
}

static void	list_channels(t_param_list params, Client *sender, Server *server)
{
	std::set<Channel *>				ch_list;
	std::set<Channel *>::iterator	it;
	Channel							*ch;

	ch_list = get_channel_list(params, server);
	it = ch_list.begin();
	while (it != ch_list.end())
	{
		ch = *(it++);
		sender->sendReply(RPL_LIST, ch->getName() + " " + utostr(ch->getUserList().size()) + " :" + ch->getTopic());
	}
}

void	com_List(t_param_list params, Client *sender, Server *server)
{
	if (!sender->getRegistered())
		return sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
	sender->sendReply(RPL_LISTSTART, "Channel :Users  Name");
	list_channels(params, sender, server);
	sender->sendReply(RPL_LISTEND, ":End of /LIST");
}
