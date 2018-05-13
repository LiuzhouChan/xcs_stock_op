
#ifndef XCS_ACTION_BASE_HPP
#define XCS_ACTION_BASE_HPP

#include <string>
#include "xcs_random.hpp"



template <class _action>
class action_base
{
protected:
	/*! \var unsigned long action
	 *  \brief index of the action represented
	 */
	unsigned long action;	

public:
	/*!
	 * \fn string class_name() const
	 * \brief name of the class that implements the action set
	 * 
	 * This function returns the name of the class. 
	 * Every class must implement this method that is used to 
	 * trace errors.
	 */

	virtual std::string class_name() const = 0;

	//! tag used to access the configuration file
	virtual std::string tag_name() const = 0;

	//! number of available actions (i.e., |A|)
	virtual unsigned long actions() const = 0;

	//! generate a random action 
	virtual void random() {set_value(xcs_random::dice(actions()));};

	//! mutate the action 
	virtual void mutate(const double& mu) = 0;

	//! set the action to the first available one (i.e., A0)
	virtual void reset_action() {set_value(0);}; 

	//! set the action to the next available one; it returns false if there are no more available actions.
	virtual bool next_action() 
	{ 
		//!
	 	if (value()+1<(actions())) 
		{
			set_value(value()+1);
			return true;
		} else {
			return false;
		}
	}; 

	//! return the integer action value
	virtual unsigned long value() const { return action; };

	//! set the action value
	virtual void set_value(unsigned long act) { action = act; };

	//! number of available actions
	virtual unsigned long	size() const { return actions(); };

	//! return the action value as a string
	virtual std::string string_value() const = 0;

	//! set the action value from a string
	virtual void set_string_value(std::string str) = 0;

	//! equality operator
	//bool operator==(const _action& act) const { return (value()==act.value()); };

	//@{ comparison operators
	
	//! equality operator
	virtual bool operator==(const _action& act) const { return (value()==act.value()); };
	//! less than operator
	virtual bool operator< (const _action& act) const { return (value()<act.value()); };
	//! not equal operator
	virtual bool operator!=(const _action& act) const { return (value()!=act.value()); };
	//@}

	//! read the action from an input stream 
	friend std::istream& operator>>(std::istream& input, _action& action) 
	{
		std::string	str;
		if(input >> str)
			action.set_string_value(str);
		return (input);
	};

	//! write the action to an output stream 
	friend std::ostream& operator<<(std::ostream& output, const _action& action)
	{
		std::string str;

		str = action.string_value();
		output << str;
		return (output);
	};

	//! assignment operators
	_action& operator=(_action& act) {set_value(act.value()); return (*this);};
	_action& operator=(const _action& act) {set_value(act.value()); return (*this);};

	//! print the action.
	virtual void print(std::ostream &output) const {output << string_value();};
	
	//! version
	std::string version() const {return "no information available";};
};

#endif // !XCS_ACTION_BASE_HPP