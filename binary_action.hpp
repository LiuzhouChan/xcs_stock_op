
#ifndef XCS_BINARY_ACTION_HPP
#define XCS_BINARY_ACTION_HPP

#include "action_base.hpp"
#include "util.hpp"
#include "config_magr2.hpp"


class binary_action : public virtual action_base<binary_action>
{
private:
	static bool				init;
	static unsigned long	no_actions;
	static unsigned long	no_bits;
	std::string				bitstring;

public:
	std::string class_name() const { return std::string("binary_action"); };
	std::string tag_name() const { return std::string("action::binary"); };

	binary_action();
	binary_action(int);
	binary_action(config_mgr2&);

	~binary_action() = default;

	unsigned long actions() const;

	void set_value(unsigned long);
	std::string string_value() const;
	void set_string_value(std::string);

	void random();
	void mutate(const double&); 
};



#endif // !XCS_BINARY_ACTION_HPP