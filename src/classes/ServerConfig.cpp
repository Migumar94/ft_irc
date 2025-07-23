#include <ServerConfig.hpp>

#include <fstream>	// File management
#include <stdlib.h>	// atoi

#define CONFIG_PATH "./conf/server.conf"
#define BUFFER_SIZE 1024

#include <iostream>

ServerConfig::ServerConfig():_network_name("localhost"),_servername("ft_irc"),_version("0.0.0"),_welcome_msg("Welcome to " + _network_name + "!"),_max_channels(0),_max_users(1024),_max_users_per_channel(0),_max_channels_per_user(10)
{
	readConfig();
	#ifdef DEBUG_MODE
	std::cout << "----- CONFIG START -----\n";
	std::cout << "Network name: " << _network_name << "\n";
	std::cout << "Server name: " << _servername << "\n";
	std::cout << "Version: " << _version << "\n";
	std::cout << "Welcome message: " << _welcome_msg << "\n";
	std::cout << "Max channels: " << _max_channels << "\n";
	std::cout << "Max users: " << _max_users << "\n";
	std::cout << "Max users per channel: " << _max_users_per_channel << "\n";
	std::cout << "Max channels per user: " << _max_channels_per_user << "\n";
	std::cout << "----- CONFIG END -----\n";
	#endif
}

ServerConfig::~ServerConfig(){}

std::string		ServerConfig::getNetworkName(){return _network_name;}
std::string		ServerConfig::getServerName(){return _servername;}
std::string		ServerConfig::getVersion(){return _version;}
std::string		ServerConfig::getWelcomeMsg(){return _welcome_msg;}
void			ServerConfig::setWelcomeMsg(std::string msg){_welcome_msg = msg;}
unsigned int	ServerConfig::getMaxChannels() const {return _max_channels;}
unsigned int	ServerConfig::getMaxUsers() const {return _max_users;}
unsigned int	ServerConfig::getMaxUsersPerChannel() const {return _max_users_per_channel;}
unsigned int	ServerConfig::getMaxChannelsPerUser() const {return _max_channels_per_user;}

static std::string	getKey(std::string &str)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;
	std::string		key;

	i = 0;
	while (i < str.size() && isspace(str.at(i)))
		i++;
	j = i;
	while (j < str.size() && !isspace(str.at(j)) && str.at(j) != '=')
		j++;
	k = j;
	while (k < str.size() && isspace(str.at(k)) && str.at(k) != '=')
		k++;
	if (k == str.size() || str.at(k) != '=')
		key = std::string();
	else
	{
		key = str.substr(i, j);
		str = str.substr(k + 1);
	}
	return key;
}

static std::string	getValue(std::string str)
{
	unsigned int	i;
	unsigned int	j;
	std::string		value;
	bool			quoted;

	i = 0;
	while (i < str.size() && isspace(str.at(i)))
		i++;
	quoted = false;
	j = i;
	while (j < str.size() && (quoted || !isspace(str.at(j))))
	{
		if (!quoted && str.at(j) == '\"')
			quoted = true;
		else if (quoted && str.at(j) == '\"')
			quoted = false;
		else
			value += str.at(j);
		j++;
	}
	return value;
}

void	ServerConfig::readConfig()
{
	std::ifstream			file(CONFIG_PATH);
	char					buffer[BUFFER_SIZE];
	std::string				str;
	std::string				key;
	std::string				value;

	if (!file)
	{
		std::cerr << "Error. Cannot open server config file.\n";
		return ;
	}
	while (!file.eof())
	{
		file.getline(buffer, BUFFER_SIZE);
		str = buffer;
		if (str.empty() || str.at(0) == '#')
			continue ;
		key = getKey(str);
		if (key.empty())
			continue;
		value = getValue(str);
		if (value.empty())
			continue;
		if (key == "max_channels")
			_max_channels = atoi(value.c_str());
		else if (key == "max_users")
			_max_users = atoi(value.c_str());
		else if (key == "max_users_per_channel")
			_max_users_per_channel = atoi(value.c_str());
		else if (key == "max_channels_per_user")
			_max_channels_per_user = atoi(value.c_str());
		else if (key == "welcome_message")
			_welcome_msg = value;
		else if (key == "network_name")
			_network_name = value;
		else if (key == "server_name")
			_servername = value;
		else if (key == "server_version")
			_version = value;
		else
			std::cerr << "Server config: Unknown option '" << key << "'\n.";
	}
	file.close();
}
