#ifndef CHANNEL_MODE_HPP
#define	CHANNEL_MODE_HPP

# include <string>
# include <set>

class ChannelMode
{
	private:
		bool					_invite_only;		// +i
		std::set<std::string>	_invite_list;
		std::string				_password;			// +k
		unsigned int			_max_users; 		// +l, 0 = no limit
		bool					_outside_forbidden;	// +n
		std::set<std::string>	_operators;			// +o
		bool					_topic_restricted;	// +t

	public:
		ChannelMode(unsigned int max_users);
		~ChannelMode();

		void	setInviteOnly(bool b);
		bool	isInviteOnly() const;

		void	addToInviteList(std::string nick);
		void	removeFromInviteList(std::string nick);
		bool	inInviteList(std::string nick) const;

		bool		passwordMatch(std::string password) const;
		void		setPassword(std::string password);
		std::string	getPassword() const;
		
		void			setMaxUsers(unsigned int max);
		unsigned int	getMaxUsers() const;

		void	setOutsideForbidden(bool b);
		bool	isOutsiteForbidden() const;

		bool	hasOperator(std::string const nick) const;
		void	addOperator(std::string const nick);
		void	removeOperator(std::string const nick);

		void	setTopicRestricted(bool b);
		bool	isTopicRestricted() const;

};

#endif