#include <string>

static void	add_ciphers(std::string &str, unsigned int n)
{
	if (n > 9)
	{
		add_ciphers(str, n / 10);
		add_ciphers(str, n % 10);
	}
	else
		str += (char)(n + '0');
}

std::string	utostr(unsigned int n)
{
	std::string	str;

	add_ciphers(str, n);
	return str;
}

std::string	capitalize(std::string str)
{
	std::string					out;
	std::string::const_iterator	it;

	it = str.begin();
	while (it != str.end())
	{
		if (*it >= 'a' && *it <= 'z')
			out += *it + 'A' - 'a';
		else
			out += *it;
		it++;
	}
	return out;
}
