#include <iostream>
#include <string>
#include <Server.hpp>
#include <cstdlib>

static int	check_port(char *port_str, int &port)
{
	unsigned int	i;

	i = 0;
	while (port_str[i] && port_str[i] >= '0' && port_str[i] <= '9')
		i++;
	if (port_str[i])
	{
		std::cerr << "Invalid port argument \"" << port_str << "\"." << std::endl;
		return 1;
	}
	port = atoi(port_str);
	if (port < 0 || port > 65535)
	{
		std::cerr << "Invalid port number." << std::endl;
		return 1;
	}
	return 0;
}

static int	check_password(char *str, std::string &password)
{
	unsigned int	i;

	if (!str)
	{
		password = std::string();
		return 0;
	}
	i = 0;
	while (str[i] && !isspace(str[i]))
		i++;
	if (str[i])
	{
		std::cerr << "Invalid password \"" << str << "\"" << std::endl;
		return 1;
	}
	password = str;
	return 0;
}

static int	check_args(int argc, char **argv, int &port, std::string &password)
{
	if (argc < 2 || argc > 3)
	{
		std::cout << "Usage: " << argv[0] << " <port> [password]" << std::endl;
		return 1;
	}
	if (check_port(argv[1], port))
		return 1;
	return check_password(argv[2], password);
}

int main(int argc, char* argv[])
{
	int			port;
	std::string	password;
	int			result;

	if (check_args(argc, argv, port, password))
		return 1;
	result = Server().start(port, password);
	return result;
}
