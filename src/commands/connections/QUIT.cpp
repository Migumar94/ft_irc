#include <commands.hpp>
#include <iostream>

static void	send_quit_message(std::string message, Client *sender)
{
	std::set<Client *>	user_list;
	std::set<Channel *>	channel_list;
	std::set<Client *>	channel_users_list;
	Client				*client;

	channel_list = sender->getActiveChannels();
	for (std::set<Channel *>::iterator ch_it = channel_list.begin(); ch_it != channel_list.end(); ch_it++)
	{
		channel_users_list = (*ch_it)->getUserList();
		for (std::set<Client *>::iterator cl_it = channel_users_list.begin(); cl_it != channel_users_list.end(); cl_it++)
		{
			client = *cl_it;
			if (client == sender)
				continue ;
			user_list.insert(client);
		}	
	}
	if (!message.empty())
		message = " :" + message;
	for (std::set<Client *>::iterator cl_it = user_list.begin(); cl_it != user_list.end(); cl_it++)
		(*cl_it)->sendText(":" + sender->getNickName() + " QUIT" + message);
}

void	com_Quit(t_param_list params, Client *sender, Server *server)
{
	std::string	message;
	if (!sender->getRegistered())
	{
		sender->sendReply(ERR_NOTREGISTERED, ":You have not registered");
		server->deleteClient(sender);
		return ;
	}
	if (!params.empty())
		message = params.at(0);
	send_quit_message(message, sender);
	server->deleteClient(sender);
}
