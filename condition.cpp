
#include "condition.hpp"
#include "xcs_random.hpp"
#include "config_magr2.hpp"

#include <string>
#include <fstream>
#include <algorithm>
#include <cassert>

using namespace std;



//
//	condition parameters
//
bool		ternary_condition::init = false;	
unsigned long	ternary_condition::no_bits;
double		ternary_condition::dont_care_prob;
unsigned long	ternary_condition::crossover_type;
unsigned long	ternary_condition::mutation_type;
bool		ternary_condition::flag_mutation_with_dontcare;

const char	dont_care = '#';

ternary_condition::ternary_condition()
{
	if (!ternary_condition::init)
	{
		error(class_name(),"ternary_condition()", "not inited", 1);
	}

}

ternary_condition::ternary_condition(config_mgr2& xcs_config)
{
	string		str_mutation_with_dontcare;
	ifstream 	config;
	
	if (!ternary_condition::init)
	{
		if (!xcs_config.exist(tag_name()))
		{
			error(class_name(), "constructor", "section <" + tag_name() + "> not found", 1);	
		}
		
		try {
			no_bits = xcs_config.Value(tag_name(), "condition size");
			dont_care_prob = xcs_config.Value(tag_name(), "dontcare probability");
			str_mutation_with_dontcare = (string) xcs_config.Value(tag_name(), "mutate with dontcare");
			crossover_type = xcs_config.Value(tag_name(), "crossover method");
			mutation_type = xcs_config.Value(tag_name(), "mutation method");
		} catch (const char *attribute) {
			string msg = "attribute \'" + string(attribute) + "\' not found in <" + tag_name() + ">";
			error(class_name(), "constructor", msg, 1);
		}

		if ((mutation_type<1)||(mutation_type>3))
		{
			error(class_name(),"class constructor", "mutation type can be 1, 2, or 3", 1);
		}

		if ((crossover_type<0)||(crossover_type>2))
		{
			error(class_name(),"class constructor", "crossover type can be 0, 1, or 2", 1);
		}
		set_flag(string(str_mutation_with_dontcare), ternary_condition::flag_mutation_with_dontcare);
	}
	init = true;
};

bool  ternary_condition::operator<(const ternary_condition& cond) const
{
	return (bitstring<cond.string_value());
};

bool ternary_condition::operator==(const ternary_condition& cond) const
{
	return (bitstring==cond.string_value());
};

bool ternary_condition::operator!=(const ternary_condition& cond) const
{
	return (bitstring!=cond.string_value());
}

/*!
 *	assignment operators
 */
ternary_condition& ternary_condition::operator=(ternary_condition& cond)
{
	bitstring = cond.string_value();
	return (*this);
}

ternary_condition& ternary_condition::operator=(const ternary_condition& cond)
{
	bitstring = cond.string_value();
	return (*this);
}

// 
// 	match operator
//
bool ternary_condition::match(const binary_inputs& sens) const
{
	string::size_type	bit;
	string			input;
	bool			result;

	input = sens.string_value();

	assert(input.size()==bitstring.size());

	bit = 0;
	result = true;

	while ( (result) && (bit<bitstring.size()) )
	{
		result = ( (bitstring[bit]=='#') || (bitstring[bit]==input[bit]) );
		bit++;
	}
	
	return result;
}

// 
// 	cover operator
//
void ternary_condition::cover(const binary_inputs& sens) 
{
	string::size_type	bit;
	string			input;

	input = sens.string_value();

	bitstring = "";

	for(bit = 0; bit<input.size(); bit++)
	{
		if (xcs_random::random()<ternary_condition::dont_care_prob)
		{
			//bitstring += ternary_condition::dont_care;
			bitstring += '#';
		} else {
			bitstring += input[bit];
		}
	}
}

// 
// 	mutate operator
//
void ternary_condition::mutate(const binary_inputs &sens, const double& mu)
{
	//! if mutation==1, restricted (1-value) mutation is required
	if (mutation_type==1)
	{
		string::size_type	bit;
		string			input;
	
		input = sens.string_value();
	
		assert(input.size()==bitstring.size());
	
		for(bit = 0; bit<bitstring.size(); bit++)
		{
			if (xcs_random::random()<mu)
			{
				if (bitstring[bit]=='#')
				{
					bitstring[bit] = input[bit];
				} else {
					if (flag_mutation_with_dontcare)
						bitstring[bit] = '#';
				}
			}
		}
	} else {
		mutate(mu);
	}
}

// 
// 	mutate operator
//
void ternary_condition::mutate(const double& mu)
{
	string::size_type	bit;

	for(bit = 0; bit<bitstring.size(); bit++)
	{
		if (xcs_random::random()<mu)
		{
			//! pure random (3 values) mutation
			if (mutation_type==3)
			{
				if (flag_mutation_with_dontcare)
				{
					char values[] = {'0','1','#'};
					bitstring[bit] = values[xcs_random::dice(3)];
				} else {
					bitstring[bit] = '0'+xcs_random::dice(2);
				}
			//! mutate the bit to one of the other two values
			} else if (mutation_type==2) {
				if (bitstring[bit]=='#')
				{
					bitstring[bit] = '0'+xcs_random::dice(2);
				} else {
					if (flag_mutation_with_dontcare)
					{
						char	values[2] = {'?', '#'};
						values[0] = ('1' - bitstring[bit])+'0';
						bitstring[bit] = values[xcs_random::dice(2)];
					} else {
						bitstring[bit] = ('1' - bitstring[bit])+'0';
					}
				}
			} else {
				error(class_name(),"mutate()", "mutation type not allowed", 1);
			}
		}
	}
}

void ternary_condition::recombine(ternary_condition& offspring, unsigned long method)
{
	assert( (method>=0) && (method<3) );

	switch(method)
	{
		case 0:
			uniform_crossover(offspring);
			break;
		case 1:
			single_point_crossover(offspring);
			break;
		case 2:
			two_point_crossover(offspring);
			break;
	}
}

bool ternary_condition::is_more_general_than(const ternary_condition& cond) const
{
	string		str = cond.string_value();
	unsigned long 	bit;
	unsigned long	sz = bitstring.size();
	bool		does_subsume;

	bit = 0;
	does_subsume = true;
	while  ( (bit<sz) && (does_subsume) )
	{
		does_subsume = ((bitstring[bit]=='#') || (str[bit]==bitstring[bit]));
		bit++;
	}

	return does_subsume;
}

void ternary_condition::random() 
{
	string::size_type	bit;

	bitstring = "";

	for(bit = 0; bit<ternary_condition::no_bits; bit++)
	{
		if (xcs_random::random()<ternary_condition::dont_care_prob)
		{
			bitstring += '#';
		} else {
			bitstring += '0' + xcs_random::dice(2);
		}
	}
}

/// single point crossover
void ternary_condition::single_point_crossover(ternary_condition& offspring)
{
    unsigned long sz = size();
    unsigned long point = 1+xcs_random::dice(sz-1);
	string str = offspring.string_value();

	for(unsigned long bit=point; bit<sz; bit++)
	{
		swap(bitstring[bit],str[bit]);
	}
	offspring.set_string_value(str);
}

//! two point crossover
void ternary_condition::two_point_crossover(ternary_condition& offspring)
{
	unsigned long	bit;
	string		str = offspring.string_value();

	unsigned long	x;
	unsigned long	y;

	x = xcs_random::dice(offspring.size()+1);
	y = xcs_random::dice(size()+1);
	if (x>y)
		swap(x,y);
	bit = 0;

	do {
		if ((x<bit) && (bit<y))
			swap(bitstring[bit], str[bit]);
		bit++;
	} while ( bit<y );

	offspring.set_string_value(str);

}

//! uniform crossover
void ternary_condition::uniform_crossover(ternary_condition& offspring)
{
        unsigned long sz = size();

	string str = offspring.string_value();

	for(unsigned long bit=0; bit<sz; bit++)
	{
		if (xcs_random::random()<.5)
			swap(bitstring[bit],str[bit]);
	}
	offspring.set_string_value(str);
}

//! generality
double ternary_condition::generality() const
{
	return 1-specificity();
};

//! specificity
double ternary_condition::specificity() const
{
	string::const_iterator	ch;
	
	double specificity = 0;
	
	for( ch=bitstring.begin(); ch!=bitstring.end(); ch++ )
	{
		specificity += (*ch!='#')?1:0;	
	}

	specificity = specificity/bitstring.size();

	return specificity;
};
