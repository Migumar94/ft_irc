#include <Client.hpp>
#include <iostream>
#include <Request.hpp>
#include <utils.hpp>

Client::Client(Server *server):_server(server),_password_matched(false),_registered(false),_away(false)
{
	if (server->passwordMatch(std::string()))
		_password_matched = true;
}

Client::~Client()
{
	while (_active_channels.size())
		removeChannel(*_active_channels.begin());
	_server->deleteClient(this);
}

pollfd	*Client::getPollFd(){return _pfd;}
void	Client::setPollFd(pollfd *pfd){_pfd = pfd;}

void	Client::processRequests()
{
	char 		buffer[513] = {0};
	const char	*buffer_ptr;
	int			bytes_received;
	Request		r;
	t_com_func	f;

	buffer_ptr = buffer;
	bytes_received = recv(_pfd->fd, buffer, 513, 0);
	if (buffer[512])
	{
		std::cout << "Message is too long";
		return ;
	}
	_pfd->revents = 0;
	if (bytes_received <= 0)
	{
		if (_nickname.empty())
			std::cout << "Client " << _pfd->fd << " disconnected\n";
		else
			std::cout << _nickname << " disconnected\n";
		_server->deleteClient(this);
	}
	else
	{
		#ifdef DEBUG_MODE
		if (_nickname.empty())
			std::cout << "Request from " << _pfd->fd << ":\n\t[" << buffer << "]\n";
		else
			std::cout << "Request from " << _nickname << ":\n\t[" << buffer << "]\n";
		#endif
		// Execute all the messages un the buffer
		while (*buffer_ptr)
		{
			r = Request(&buffer_ptr);
			f = r.getCommand();
			if (f)
				f(r.getParams(), this, _server);
			else
				this->sendReply(ERR_UNKNOWNCOMMAND, r.getCommandName() + " :Unknown command");
		}
	}
}

void		Client::setPasswordMatched(){_password_matched = true;}
bool		Client::getPasswordMatched(){return _password_matched;}

void		Client::setNickName(const std::string nickname){_nickname = nickname;}
std::string	Client::getNickName() const{return _nickname;}

void		Client::setUserName(const std::string username){_username = username;}
std::string	Client::getUserName() const{return _username;}

void		Client::setRealName(const std::string realname){_real_name = realname;}
std::string	Client::getRealName() const{return _real_name;}

bool		Client::getRegistered(){return _registered;}
void		Client::setRegistered(){_registered = true;}

bool	Client::isOperator(){return _server->hasOperator(this->getNickName());}
bool	Client::isChannelOperator(Channel *channel){return channel->getMode()->hasOperator(this->getNickName());}

void	Client::addChannel(Channel * const channel)
{
	if (!channel)
		return ;
	if (this->isInChannel(channel))
		return ;
	_active_channels.insert(channel);
	channel->addUser(this);
}

void	Client::removeChannel(Channel * const channel)
{
	if (!channel)
		return ;
	if (!this->isInChannel(channel))
		return ;
	_active_channels.erase(channel);
	channel->removeUser(this);
	if (!channel->getUserList().size())
		_server->deleteChannel(channel);
}

std::set<Channel *>	& Client::getActiveChannels(){return _active_channels;}

bool	Client::isInChannel(Channel * const channel) const
{
	if (_active_channels.find(channel) == _active_channels.end())
		return false;
	else
		return true;
}

void	Client::setAway(bool b){_away = b;}
bool	Client::isAway(){return _away;}
void	Client::setAwayMessage(std::string message){_away_message = message;}
std::string	Client::getAwayMessage(){return _away_message;}

void	Client::sendReply(const char * code, std::string message)
{
	std::string	str;
	std::string	servername;
	
	if (!code)
	return ;
	servername = _server->getConfig().getServerName();
	str = ":";
	if (servername.empty())
		str += "Server";
	else
		str += servername;
	str = str + " " + code + " " + (getNickName().empty() ? "0" : getNickName());
	if (!message.empty())
		str += " " + message;
	this->sendText(str);
}

void	Client::sendText(std::string text)
{
	int			bytes;
	
	if (text.empty())
		return ;
	#ifdef DEBUG_MODE
	std::string	target;
	if (_nickname.empty())
		target = "" + utostr(this->getPollFd()->fd);
	else
		target = _nickname;
	std::cout << "Responding to " << target << ":\n\t[" << text << "]\n";
	#endif
	text += "\r\n";
	bytes = send((*this->getPollFd()).fd, text.c_str(), text.size(), 0);
	if (bytes == -1)
		std::cerr << "Cannot send reply to the client " << this->getNickName() << ".\n";
}