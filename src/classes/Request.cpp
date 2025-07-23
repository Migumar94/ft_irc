#include <Request.hpp>
#include <iostream>
#include <utils.hpp>

Request::Request():_command(NULL){}

Request::Request(const char ** message):_command(NULL)
{
	_prefix = std::string();
	_command = NULL;
	_params = std::vector<std::string>();

	if (!*message)
		return;
	setPrefix(message);
	setCommand(message);
	setParams(message);
	if (!_command)
	{
		_params.clear();
		_prefix.clear();
	}
}

Request::~Request(){}

void	Request::setPrefix(const char **message)
{
	if (**message != ':' || **message != '\r' || **message != '\n')
		return;
	(*message)++;
	while (**message && !isspace(**message))
		_prefix += *((*message)++);
	while (isspace(**message) && **message != '\r' && **message != '\n')
		(*message)++;
}

void	Request::setCommand(const char **message)
{
	std::string	com_str;

	if (isspace(**message))
		return ;
	while (**message && !isspace(**message))
		com_str += *((*message)++);
	while (isspace(**message) && **message != '\r' && **message != '\n')
		(*message)++;
	_command = validCommand(com_str);
	if (!_command)
		std::cout << "Invalid command \"" << com_str << "\".\n";
}

void	Request::setParams(const char **message)
{
	std::string	param;

	if ((*message)[0] == '\r' && (*message)[1] == '\n')
	{
		*message += 2;
		return ;
	}
	while (**message && **message != ':' && **message != '\r' && **message != '\n')
	{
		param = std::string();
		while (!isspace(**message))
			param += *((*message)++);
		while (isspace(**message) && **message != '\r' && **message != '\n')
			(*message)++;
		_params.push_back(param);
	}
	if (**message == ':')
	{
		param = std::string();
		(*message)++;
		while (**message && **message != '\r' && **message != '\n')
			param += *((*message)++);
		_params.push_back(param);
	}
	if ((*message)[0] != '\r' || (*message)[1] != '\n')
	{
		std::cout << "Format error. Message not terminated with \\r\\n.\n";
		_params.clear();
		_command = NULL;
		_prefix.clear();
	}
	*message += 2;
}

std::string					Request::getPrefix() {return _prefix;}
std::string					Request::getCommandName() {return _command_name;}
t_com_func					Request::getCommand() {return _command;}
std::vector<std::string>	Request::getParams() {return _params;}

t_com_func	Request::validCommand(const std::string str)
{
	unsigned int		i;
	const std::string	valid_commands[] = {"PASS", "NICK", "USER", "QUIT", "OPER", "JOIN", "PART", "PRIVMSG", "MODE", "INVITE",
		"KICK", "LIST", "TOPIC", "TIME", "VERSION", "WHO", "WHOIS", "NAMES", "AWAY", ""};
	const t_com_func	functions[] = {com_Pass, com_Nick, com_User, com_Quit, com_Oper, com_Join, com_Part, com_PrivMsg, com_Mode, com_Invite,
		com_Kick, com_List, com_Topic, com_Time, com_Version, com_Who, com_WhoIs, com_Names, com_Away, NULL};

	_command_name = capitalize(str);
	i = 0;
	while (!valid_commands[i].empty() && _command_name != valid_commands[i])
		i++;
	if (valid_commands[i].empty())
		return NULL;
	return functions[i];
}

/* Some functionality for the parameter analysis */
t_param_list	Request::param_split(std::string param)
{
	t_param_list			list;
	unsigned int			i;

	if (param.empty())
		return list;
	i = 0;
	while (i < param.size())
	{
		if (param.at(i) == ',')
		{
			list.push_back(param.substr(0, i));
			if (i + 1 == param.size())
				param.clear();
			else
				param = param.substr(i + 1);
			i = 0;
			continue ;
		}
		i++;
	}
	if (i)
		list.push_back(param);
	return list;
}

static bool	is_number(char c)
{
	return c >= '0' && c <= '9';
}

static bool	is_letter(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool is_special(char c)
{
	return (c == '-' || c == '[' || c == ']' || c == '\\'
		|| c == '`' || c == '^' || c == '{' || c == '}'
		|| c == '_'  // This is not standard but acceptable
	);
}

bool	Request::isNickname(std::string const str)
{
	unsigned int	i;
	unsigned int	size;
	char			c;

	size = str.size();
	if (size == 0)
		return false;
	if (!is_letter(str.at(0)))
		return false;
	i = 1;
	while (i < size)
	{
		c = str.at(i);
		if (!is_letter(c) && !is_number(c) && !is_special(c))
			return false;
		i++;
	}
	return true;
}

bool	Request::isChannel(std::string const str)
{
	unsigned int	i;
	unsigned int	size;
	char			c;

	size = str.size();
	if (size < 2 || size > 200)
		return false;
	c = str.at(0);
	if (c != '#' && c != '&')
		return false;
	i = 1;
	while (i < size)
	{
		c = str.at(i);
		if (c == '\0' || c == ' ' || c == 7 || c == 10 || c == 13 || c == ',')
			return false;
		i++;
	}
	return true;
}