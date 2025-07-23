#ifndef MODES_HPP
#define MODES_HPP

#include <commands.hpp>

int		get_limit(std::string options, bool setting, t_param_list::iterator &arg, t_param_list &params, unsigned int &new_limit);
int		get_password(std::string options, bool setting, t_param_list::iterator &arg, t_param_list &params, std::string &new_password);
int		mode_o_option(std::string options, bool setting, t_param_list::iterator &arg, t_param_list &params, Channel *channel, Client *sender);
void	mode_i_option(std::string options, bool setting, Channel *channel, Client *sender);
void	mode_n_option(std::string options, bool setting, Channel *channel, Client *sender);
void	mode_t_option(std::string options, bool setting, Channel *channel, Client *sender);
void	mode_l_option(std::string options, bool setting, Channel *channel, Client *sender, Server *server, unsigned int limit);
void	mode_k_option(std::string options, bool setting, Channel *channel, Client *sender, std::string password);

#endif