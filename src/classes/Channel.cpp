#include <Channel.hpp>
#include <iostream>

Channel::Channel(std::string name, unsigned int max_users):_name(name)
{
	std::cout << "Channel '" << name << "' created,\n";
	_mode = new ChannelMode(max_users);
}

Channel::~Channel(){delete _mode;}

std::string	Channel::getName() const{return _name;}
ChannelMode	*Channel::getMode() const{return _mode;}

void		Channel::setTopic(std::string topic){_topic = topic;}
std::string	Channel::getTopic() const {return _topic;}


void	Channel::addUser(Client * client)
{
	if (!client)
		return ;
	if (hasUser(client))
		return ;
	_user_list.insert(client);
	client->addChannel(this);
}

std::set<Client *>	Channel::getUserList(){return _user_list;}

void	Channel::removeUser(Client * client)
{
	if (!client)
		return ;
	if (!hasUser(client))
		return ;
	_user_list.erase(client);
	client->removeChannel(this);
}

bool	Channel::hasUser(Client * client)
{
	t_cl_it	it;

	if (!client)
		return false;
	it = _user_list.find(client);
	return (it != _user_list.end());
}

void	Channel::sendText(Client *sender, std::string text)
{
	t_cl_it	it;

	it = _user_list.begin();
	while (it != _user_list.end())
		if (*it != sender)
			(*(it++))->sendText(text);
		else
			it++;
}