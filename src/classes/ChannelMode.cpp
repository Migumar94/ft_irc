#include <ChannelMode.hpp>

ChannelMode::ChannelMode(unsigned int max_users):_invite_only(false),_max_users(max_users),_outside_forbidden(true),_topic_restricted(true){}
ChannelMode::~ChannelMode(){}


void	ChannelMode::setInviteOnly(bool b){_invite_only = b;}
bool	ChannelMode::isInviteOnly() const {return _invite_only;}

void	ChannelMode::addToInviteList(std::string nick)
{
	if (nick.empty())
		return ;
	if (inInviteList(nick))
		return ;
	_invite_list.insert(nick);
}

void	ChannelMode::removeFromInviteList(std::string nick)
{
	if (!inInviteList(nick))
		return ;
	_invite_list.erase(nick);
}

bool	ChannelMode::inInviteList(std::string nick) const
{
	std::set<std::string>::iterator it;

	it = _invite_list.find(nick);
	return (it != _invite_list.end());
}

void		ChannelMode::setPassword(std::string password){_password = password;}
std::string	ChannelMode::getPassword() const {return _password;}
bool		ChannelMode::passwordMatch(std::string password) const {return (password == _password);}

void			ChannelMode::setMaxUsers(unsigned int max){_max_users = max;}
unsigned int	ChannelMode::getMaxUsers() const {return _max_users;}

void	ChannelMode::setOutsideForbidden(bool b){_outside_forbidden = b;}
bool	ChannelMode::isOutsiteForbidden() const{return _outside_forbidden;}

bool	ChannelMode::hasOperator(const std::string nick) const
{
	std::set<std::string>::iterator it;

	it = _operators.find(nick);
	return (it != _operators.end());
}

void ChannelMode::addOperator(std::string const nick)
{
	if (hasOperator(nick))
		return ;
	_operators.insert(nick);
}

void ChannelMode::removeOperator(std::string const nick)
{
	if (!hasOperator(nick))
		return ;
	_operators.erase(nick);
}

void	ChannelMode::setTopicRestricted(bool b){_topic_restricted = b;}
bool	ChannelMode::isTopicRestricted() const {return _topic_restricted;}
