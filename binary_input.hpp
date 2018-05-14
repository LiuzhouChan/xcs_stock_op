
#ifndef XCS_BINARY_INPUT_HPP
#define XCS_BINARY_INPUT_HPP

#include <iostream>
#include <string>

#include "util.hpp"


class binary_inputs
{
private:
	//! current state value
	std::string	value;

public:
	//! constructor
	binary_inputs() {};
	binary_inputs(std::string value) { set_string_value(value);};

	//! destructor
	virtual ~binary_inputs() {};

	//! name of the class that represents the state
	std::string class_name() { return ("binary_inputs"); };

    std::string tag_name() { return ""; };
	//! return the size of the state; for instance, with binary representation, returns the number of bits.
	unsigned long	size() const {return value.size();};

	//! set the value of the state from a string
	std::string string_value() const { return value; };

	//! set the value of the state from a string
	void set_string_value(const std::string &str);

	//! return the value of the specified input bit
	char input(unsigned long) const;

	//! set the value of a specific input
	void set_input(unsigned long, char);

	//! equality operator 
	bool operator==(const binary_inputs& st) const {
		return (string_value()==st.string_value());
	};

	//! not equal operator
	bool operator!=(const binary_inputs& st) const {
		return (string_value()!=st.string_value());
	};

	//! assignment operators
	binary_inputs& operator=(binary_inputs& st) { value = st.string_value(); return (*this); };
	binary_inputs& operator=(const binary_inputs& st) { value = st.string_value(); return (*this); };

    friend std::ostream& operator<<(std::ostream& output, const binary_inputs& st) {
		output << st.string_value();
		return (output);
	};

	//! read the sensors from an input stream 
	friend std::istream& operator>>(std::istream& input, binary_inputs& st) {
		std::string	str;
		input >> str;
		st.set_string_value(str);
		return (input);
	};

	//! return true if the sensory inputs can be represented as a vector of long
	bool allow_numeric_representation() const { return true; };

	//! return a vector of long that represents the sensory inputs
	void numeric_representation(std::vector<long>&) const;
	void numeric_representation(std::vector<double>&) const;

	//! return a vector of long that represents the sensory inputs
	void set_numeric_representation(const std::vector<long>&);
	void set_numeric_representation(const std::vector<double>&);
};

#endif // !XCS_BINARY_INPUT_HPP
