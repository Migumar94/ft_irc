#include <OperatorList.hpp>

#include <fstream>
#include <iostream>
#define OPER_PATH "./conf/operators.conf"
#define BUFFER_SIZE 1024

static std::string	getKey(std::string &str)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;
	std::string		key;

	i = 0;
	while (i < str.size() && isspace(str.at(i)))
		i++;
	j = i;
	while (j < str.size() && !isspace(str.at(j)) && str.at(j) != '=')
		j++;
	k = j;
	while (k < str.size() && isspace(str.at(k)) && str.at(k) != '=')
		k++;
	if (k == str.size() || str.at(k) != '=')
		key = std::string();
	else
	{
		key = str.substr(i, j);
		str = str.substr(k + 1);
	}
	return key;
}

static std::string	getValue(std::string str)
{
	unsigned int	i;
	unsigned int	j;
	std::string		value;
	bool			quoted;

	i = 0;
	while (i < str.size() && isspace(str.at(i)))
		i++;
	quoted = false;
	j = i;
	while (j < str.size() && (quoted || !isspace(str.at(j))))
	{
		if (!quoted && str.at(j) == '\"')
			quoted = true;
		else if (quoted && str.at(j) == '\"')
			quoted = false;
		else
			value += str.at(j);
		j++;
	}
	return value;
}

void	OperatorList::readFile()
{
	std::ifstream			file(OPER_PATH);
	char					buffer[BUFFER_SIZE];
	std::string				str;
	std::string				key;
	std::string				value;

	if (!file)
	{
		std::cerr << "Error. Cannot open operators file.\n";
		return ;
	}
	while (!file.eof())
	{
		file.getline(buffer, BUFFER_SIZE);
		str = buffer;
		if (str.empty() || str.at(0) == '#' || str.at(0) == '\n')
			continue ;
		key = getKey(str);
		if (key.empty())
			continue;
		value = getValue(str);
		if (value.empty())
			continue;
		_table.insert(std::pair<std::string, std::string>(key, value));
	}
	file.close();
}

OperatorList::OperatorList(){readFile();}

OperatorList::~OperatorList(){}

bool	OperatorList::validateOper(std::string user, std::string password)
{
	std::map<std::string, std::string>::const_iterator	entry;

	for (entry = _table.begin(); entry != _table.end(); entry++)
		if ((*entry).first == user && (*entry).second == password)
			return true;
	return false;
}
