
#ifndef XCS_GENERIC_HPP 
#define XCS_GENERIC_HPP

#include <string>
#include <iostream>

class generic 
{
public:
    generic(){}
    explicit generic(const std::string&);
	explicit generic(double);
	explicit generic(long);
	explicit generic(unsigned long);
	explicit generic(const char*);
	
	generic(const generic&);
	generic& operator=(generic const&);
	generic& operator=(double);
	generic& operator=(std::string const&);

public:
	operator std::string	() const;
	operator double		() const;	
	operator unsigned long	() const;
	operator long		() const;
	
	//! write the condition to an output stream 
	friend std::ostream& operator<<(std::ostream& output, const generic& ch)
	{
		output << ch.value_;
		return (output);
	};
	
	//! read the condition from an input stream 
	friend std::istream& operator>>(std::istream& input, generic& ch)
	{
		std::string str;
		input >> str;
		ch.value_ = str; 
		return (input);
	};
	
private:
	std::string value_;
};

#endif // !XCS_GENERIC_HPP 