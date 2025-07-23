#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#include <vector>
#include <map>
#include <set>

#include <Client.hpp>
#include <Channel.hpp>
#include <ServerConfig.hpp>
#include <OperatorList.hpp>
#include <AntiSpamBot.hpp>

class Channel;
class Client;
class AntiSpamBot;

class Server
{
	private:
		AntiSpamBot							*_bot;
		std::vector<pollfd>					_all_pfds;
		pollfd								*_pollfd;

		ServerConfig						_config;
		std::string							_password;
		OperatorList						_operators;
		
		std::map<std::string, Channel *>	_channel_map;
		std::vector<Client *>				_user_list;
		std::set<std::string>				_operator_list;

		void	deleteChannels();
		void	deleteClients();

		void	refresh_pfds();
		int		open_socket(u_int16_t port);
		void	check_connections();
		void	check_client_messages();

		typedef std::map<std::string, Channel*>::iterator	t_ch_it;
		typedef std::vector<Client*>::const_iterator		t_cl_it;
	public:

		Server();
		~Server();
		
		AntiSpamBot	*getBot();

		ServerConfig	getConfig();

		bool	passwordMatch(const std::string pwd);
	
		Channel * 			createChannel(std::string name);
		void 				deleteChannel(Channel * channel);
		std::set<Channel *>	getChannelList();
		Channel *			getChannel(std::string channel_name);

		void 					createClient();
		void					deleteClient(Client * client);
		std::vector<Client *>	getUserList() const;
		Client *				getClient(std::string nickname) const;
		
		OperatorList	getOperatorList();
		bool			hasOperator(std::string nick);
		void			addOperator(std::string nick);
		void			removeOperator(std::string nick);

		int	start(u_int16_t port, std::string password);
};

#endif