#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <commands.hpp>

typedef std::vector<std::string>	t_param_list;
typedef	void(*t_com_func)(t_param_list, Client *, Server *);

class Request
{
	private:
		std::string		_prefix;
		std::string		_command_name;
		t_com_func		_command;
		t_param_list	_params;

		void	setPrefix(const char ** message);
		void	setCommand(const char ** message);
		void	setParams(const char ** message);

		t_com_func	validCommand(const std::string str);

	public:
		Request();
		Request(const char ** message);
		~Request();

		std::string		getPrefix();
		std::string		getCommandName();
		t_com_func		getCommand();
		t_param_list	getParams();

		t_param_list	param_split(std::string param);
		bool			isNickname(std::string str);
		bool			isChannel(std::string str);
};

#endif