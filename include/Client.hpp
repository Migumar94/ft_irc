#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string>

# include <Channel.hpp>
# include <Server.hpp>
# include <response_codes.hpp>
# include <set>
class Channel;
class Server;

class Client
{
	private:
		Server * const			_server;
		pollfd					*_pfd;

		bool					_password_matched;
		std::string				_nickname;
		std::string				_username;
		std::string				_real_name;
		bool					_registered;

		std::set<Channel *>		_active_channels;
		
		bool					_away;
		std::string				_away_message;

	public:
		Client(Server * server);
		~Client();

		pollfd	*getPollFd();
		void	setPollFd(pollfd *pfd);

		void	processRequests();

		void		setPasswordMatched();
		bool		getPasswordMatched();
		void		setNickName(const std::string nickname);
		std::string	getNickName(void) const;
		void		setUserName(const std::string username);
		std::string	getUserName(void) const;
		void		setRealName(const std::string realname);
		std::string	getRealName(void) const;
		void		setRegistered();
		bool		getRegistered();

		bool	isOperator();
		bool	isChannelOperator(Channel *channel);

		void				addChannel(Channel * const channel);
		void				removeChannel(Channel * const);
		std::set<Channel *>	& getActiveChannels();
		bool				isInChannel(Channel * const channel) const;

		void		setAway(bool b);
		bool		isAway();
		void		setAwayMessage(std::string message);
		std::string	getAwayMessage();

		void	sendReply(const char * code, std::string message);
		void	sendText(std::string text);
};

#endif