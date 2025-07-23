#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>

class ServerConfig
{
	private:
		std::string		_network_name;

		std::string		_servername;
		std::string		_version;
		
		std::string		_welcome_msg;
		unsigned int	_max_channels;
		unsigned int	_max_users;
		unsigned int	_max_users_per_channel;
		unsigned int	_max_channels_per_user;

		void	readConfig();
		
	public:
		ServerConfig();
		~ServerConfig();
		std::string		getNetworkName();
		std::string		getServerName();
		std::string		getVersion();
		std::string		getWelcomeMsg();
		void			setWelcomeMsg(std::string msg);
		
		unsigned int	getMaxChannels() const;
		unsigned int	getMaxUsers() const;
		unsigned int	getMaxUsersPerChannel() const;
		unsigned int	getMaxChannelsPerUser() const;
};

#endif