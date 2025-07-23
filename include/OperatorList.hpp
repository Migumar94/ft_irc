#ifndef OPERATOR_LIST_HPP
#define OPERATOR_LIST_HPP

#include <map>
#include <string>

class OperatorList
{
	private:
		std::map<std::string, std::string>	_table;
		void	readFile();
	public:
		OperatorList();
		~OperatorList();
		bool	validateOper(std::string user, std::string password);
};

#endif