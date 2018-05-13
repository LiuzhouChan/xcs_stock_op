
#include <string>
#include <iostream>
#include <cmath>

#include "binary_action.hpp"
#include "xcs_random.hpp"
#include "util.hpp"

using namespace std;

bool binary_action::init = false;
unsigned long binary_action::no_actions;
unsigned long binary_action::no_bits;

binary_action::binary_action()
{
	if (!init)
	    error(class_name(),"binary_action()", "not inited", 1);
	
}

binary_action::binary_action(int act)
{
	if (!init)
	{
	    error(class_name(),"binary_action()", "not inited", 1);
	} else {
		action = act;
		bitstring = long2binary(act, binary_action::no_bits);
	}
}


unsigned long binary_action::actions() const
{
	return binary_action::no_actions;
};


binary_action::binary_action(config_mgr2& xcs_config)
{
	if (!init)
	{
		if (!xcs_config.exist(tag_name()))
		{
			error(class_name(), "constructor", "section <" + tag_name() + "> not found", 1);	
		}
	
		try {
			
			no_bits = xcs_config.Value(tag_name(), "number of bits");

		} catch (const char *attribute) {
			string msg = "attribute \'" + string(attribute) + "\' not found in <" + tag_name() + ">";
			error(class_name(), "constructor", msg, 1);
		}

		init = true;
		
		no_actions = (unsigned long) pow(double(2),int(binary_action::no_bits));
	}
};

void binary_action::random()
{		
	string::size_type	bit;

	bitstring = "";

	for(bit=0; bit<binary_action::no_bits; bit++)
	{
		if (xcs_random::random()<.5)
			bitstring += "1";
		else
			bitstring += "0";
	}
	
	action = binary2long(bitstring);
}

void binary_action::mutate(const double& mutationRate)
{		
	string::size_type	bit;

	for(bit=0; bit<bitstring.size(); bit++)
	{
		if (xcs_random::random()<mutationRate)
		{
			if (bitstring[bit]=='0')
				bitstring[bit] = '1';
			else
				bitstring[bit] = '0';
		}
	}
	action = binary2long(bitstring);
}

string binary_action::string_value() const
{
	return bitstring;
}

void binary_action::set_string_value(string str)
{
	bitstring = str;
	action = binary2long(str);
}

void binary_action::set_value(unsigned long act) { 
	action = act;
	bitstring = long2binary(act, binary_action::no_bits);
};
