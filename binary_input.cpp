

#include <string>
#include <cassert>
#include "binary_input.hpp"


void binary_inputs::set_string_value(const std::string &str)
{
	value = str;
};

char binary_inputs::input(unsigned long pos)
const
{
	assert(pos<size());
	return value[pos];
}

void binary_inputs::set_input(unsigned long pos, char val)
{
	assert(pos<size());
	assert( (val=='0') || (val=='1') );
	value[pos] = val;
}

void binary_inputs::numeric_representation(std::vector<long>& nr) const
{
	std::string::const_iterator ch;

	nr.clear();

	for(ch=value.begin(); ch!=value.end(); ch++)
	{
		nr.push_back(long(*ch-'0'));
	}
}

void binary_inputs::set_numeric_representation(const std::vector<long>& nr)
{
	std::vector<long>::const_iterator	val;
	value = "";

	for(val=nr.begin(); val!=nr.end(); val++)
	{
		assert((*val==1)&&(*val==0));
		value += char(*val+'0');
	}
}

void
binary_inputs::numeric_representation(std::vector<double>& nr) const
{
	std::string::const_iterator ch;

	nr.clear();

	for(ch=value.begin(); ch!=value.end(); ch++)
	{
		nr.push_back(double(*ch-'0'));
	}

}

void binary_inputs::set_numeric_representation(const std::vector<double>& nr)
{
	std::vector<double>::const_iterator	val;
	value = "";
	for(val=nr.begin(); val!=nr.end(); val++)
	{
		assert((*val==1)&&(*val==0));
		value += char(*val+'0');
	}
}
