#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <Server.hpp>
#include <Client.hpp>
#include <Request.hpp>

typedef std::vector<std::string>	t_param_list;

void	com_Pass(t_param_list params, Client *sender, Server *server);
void	com_Nick(t_param_list params, Client *sender, Server *server);
void	com_User(t_param_list params, Client *sender, Server *server);
void	com_Quit(t_param_list params, Client *sender, Server *server);
void	com_Oper(t_param_list params, Client *sender, Server *server);

void	com_PrivMsg(t_param_list params, Client *sender, Server *server);
void	com_Invite(t_param_list params, Client *sender, Server *server);
void	com_List(t_param_list params, Client *sender, Server *server);
void	com_Away(t_param_list params, Client *sender, Server *server);

void	com_Join(t_param_list params, Client *sender, Server *server);
void	com_Part(t_param_list params, Client *sender, Server *server);
void	com_Kick(t_param_list params, Client *sender, Server *server);
void	com_Mode(t_param_list params, Client *sender, Server *server);
void	com_Topic(t_param_list params, Client *sender, Server *server);
void	com_Names(t_param_list params, Client *sender, Server *server);

void	com_Time(t_param_list params, Client *sender, Server *server);
void	com_Version(t_param_list params, Client *sender, Server *server);

void	com_Who(t_param_list params, Client *sender, Server *server);
void	com_WhoIs(t_param_list params, Client *sender, Server *server);


#endif