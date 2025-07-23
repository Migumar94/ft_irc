#include <Server.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h> // signals
#include <unistd.h> // close
#include <iostream>

Server::Server():_bot(new AntiSpamBot()){}

void	Server::deleteChannels()
{
	while (_channel_map.size())
		deleteChannel((*_channel_map.begin()).second);
}

void	Server::deleteClients()
{
	while (_user_list.size())
		deleteClient(*_user_list.begin());
}

Server::~Server()
{
	delete _bot;
	deleteClients();
	deleteChannels();
	if (!_all_pfds.size())
		return ;
	close((*_pollfd).fd);
	_all_pfds.erase(_all_pfds.begin());
	refresh_pfds();
}

AntiSpamBot	*Server::getBot(){return _bot;}

ServerConfig	Server::getConfig(){return _config;}

bool	Server::passwordMatch(const std::string pwd){return pwd == _password;}

Channel * Server::createChannel(const std::string name)
{
	if (getChannel(name))
		return NULL;
	Channel * ch = new Channel(name, _config.getMaxUsersPerChannel());
	if (!ch)
		return NULL;
	this->_channel_map.insert(std::pair<std::string, Channel *>(name, ch));
	return ch;
}

void Server::deleteChannel(Channel * ch)
{
	if (!ch)
		return ;
	if (!getChannel(ch->getName()))
		return ;
	t_ch_it it = _channel_map.find(ch->getName());
	_channel_map.erase(it);
	delete ch;
}

std::set<Channel *>	Server::getChannelList()
{
	std::set<Channel *>	list;

	t_ch_it it = _channel_map.begin();
	while (it != _channel_map.end())
		list.insert((*(it++)).second);
	return list;
}

Channel * Server::getChannel(const std::string channel_name)
{
	t_ch_it	it = _channel_map.find(channel_name);
	if (it == _channel_map.end())
		return (NULL);
	else
		return ((*it).second);
}

void	Server::refresh_pfds()
{
	unsigned int	i;

	_pollfd = &_all_pfds[0];
	i = 0;
	while (i < _user_list.size())
	{
		_user_list[i]->setPollFd(&_all_pfds[i + 1]);
		i++;
	}
}

void Server::createClient()
{
	Client		*client;
	int			client_fd;
	sockaddr_in	client_addr;
	socklen_t	client_len;
	pollfd		client_poll_fd;

	if (_config.getMaxUsers() && _user_list.size() >= _config.getMaxUsers())
		return;
	client_len = sizeof(client_addr);
	client_fd = accept(_all_pfds[0].fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
	{
		std::cerr << "Error accepting a new connection.\n";
		return ;
	}
	client_poll_fd.fd = client_fd;
	client_poll_fd.events = POLLIN;
	client_poll_fd.revents = 0;
	client = new Client(this);
	if (!client)
	{
		close(client_fd);
		return ;
	}
	_all_pfds.push_back(client_poll_fd);
	_user_list.push_back(client);
	refresh_pfds();
}

void Server::deleteClient(Client * client)
{
	unsigned int					i;
	std::vector<pollfd>::iterator	pfd_it;

	if (!client)
		return ;
	i = 0;
	while (i < _user_list.size() && _user_list[i] != client)
		i++;
	if (i == _user_list.size())
		return ;
	pfd_it = _all_pfds.begin() + i + 1;
	close((*pfd_it).fd);
	(*pfd_it).fd = -1;
	_user_list.erase(_user_list.begin() + i);
	_all_pfds.erase(pfd_it);
	refresh_pfds();
	delete client;
}

std::vector<Client *>	Server::getUserList() const {return _user_list;}

Client *Server::getClient(std::string nickname) const
{
	t_cl_it	it;
	it = _user_list.begin();

	if (nickname.empty())
		return NULL;
	while (it != _user_list.end())
	{
		if ((*it)->getNickName() == nickname)
			return *it;
		it++;
	}
	return NULL;
}

int	Server::open_socket(u_int16_t port)
{
	int					socket_fd;
	struct sockaddr_in	sockaddr;
	pollfd				sv_pollfd;
	int					opt;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
	{
        std::cerr << "Cannot create socket.\n";
        return 1;
    }
	// Allows to reuse the address fastly
	opt = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	// Set the server structure
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(port);
	// Bind socket
    if (bind(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
        std::cerr << "Cannot bind to port " << port << ".\n";
		close(socket_fd);
        return 1;
	}
	// Listening
	if (listen(socket_fd, _config.getMaxUsers()) < 0)
	{
		std::cerr << "Cannot listen on socket.\n";
		close(socket_fd);
		return 1;
	}
	std::cout << "ircserv listening on port " << port << "...\n";
	sv_pollfd.fd = socket_fd;
	sv_pollfd.events = POLLIN;
	sv_pollfd.revents = 0;
	_all_pfds.push_back(sv_pollfd);
	_pollfd = _all_pfds.data();
	return 0;
}

void	Server::check_connections()
{
	if (!(_all_pfds[0].revents & POLLIN))
		return ;
	createClient();
}

void	Server::check_client_messages()
{
	unsigned int	i;
	Client			*client;
	pollfd			pfd;

	i = 0;
	while (i < _user_list.size())
	{
		client = _user_list[i];
		pfd = *client->getPollFd();
		if (pfd.revents & POLLHUP)
			deleteClient(client);
		else if (pfd.revents & POLLIN)
		{
			client->processRequests();
			if (i < _user_list.size() && _user_list[i] == client)
				i++;
		}
		else
			i++;
	}
}

OperatorList	Server::getOperatorList(){return _operators;}

bool	Server::hasOperator(std::string nick)
{
	std::set<std::string>::iterator	it = _operator_list.find(nick);
	return (it != _operator_list.end());
}

void	Server::addOperator(std::string nick)
{
	if (hasOperator(nick))
		return ;
	_operator_list.insert(nick);
}

void	Server::removeOperator(std::string nick)
{
	if (!hasOperator(nick))
		return ;
	_operator_list.erase(_operator_list.find(nick));
}

static bool has_to_close = false;

static void	sigint_handler(int signum)
{
	if (signum == SIGINT)
		has_to_close = true;
}

int	Server::start(u_int16_t port, std::string password)
{
	int	n_events;

	_password = password;
	if (open_socket(port))
		return 1;
	signal(SIGINT, sigint_handler);
    while (!has_to_close)
	{
		n_events = poll(_all_pfds.data(), _all_pfds.size(), 600000);
		if (n_events == -1)
		{
			std::cerr << "Poll interrupted.\n";
			return 1;
		}
		check_connections();
		check_client_messages();
		if (_user_list.empty())
			has_to_close = true;
	}
	std::cout << "Closing server for inactivity.\n";
	return 0;
}
