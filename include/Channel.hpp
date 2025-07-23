#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <Client.hpp>
# include <ChannelMode.hpp>
# include <map>
# include <set>
# include <string>

class Client;

class Channel
{
	private:
		const std::string		_name;
		std::string				_topic;
		ChannelMode				*_mode;
		std::set<Client *>		_user_list;

	public:
		Channel(std::string name, unsigned int max_users);
		~Channel();
		
		typedef	std::set<Client *>::iterator	t_cl_it;

		std::string	getName() const;
		void		setTopic(std::string topic);
		std::string	getTopic() const;
		ChannelMode	*getMode() const;

		bool				hasUser(Client * client);
		void				addUser(Client * client);
		void				removeUser(Client * client);
		std::set<Client *>	getUserList();

		void	sendText(Client *sender, std::string text);
};

#endif