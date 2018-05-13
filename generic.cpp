


#include <cstdlib>
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>

#include "generic.hpp"


generic::generic(std::string const& value) {
  value_=value;
}

generic::generic(const char* c) {
  value_=c;
}

generic::generic(double d) {
  std::stringstream s;
  s<<d;
  value_=s.str();
}

generic::generic(unsigned long d) {
  std::stringstream s;
  s<<d;
  value_=s.str();
}

generic::generic(long d) {
  std::stringstream s;
  s<<d;
  value_=s.str();
}

generic::generic(generic const& other) {
  value_=other.value_;
}

generic& generic::operator=(generic const& other) {
  value_=other.value_;
  return *this;
}

generic& generic::operator=(double i) {
  std::stringstream s;
  s << i;
  value_ = s.str();
  return *this;
}

generic& generic::operator=(std::string const& s) 
{
	value_=s;
	return *this;
}

generic::operator std::string() const 
{
	return value_;
}

generic::operator double() const 
{
#ifdef __DEBUG__
	std::cout << ">>STRING <" << value_.c_str() << "> TO " << atof(value_.c_str()) << std::endl;
#endif
	return atof(value_.c_str());
}


generic::operator long() const 
{
#ifdef __DEBUG__
	std::cout << ">>STRING <" << value_.c_str() << "> TO " << atol(value_.c_str()) << std::endl;
#endif
	return atol(value_.c_str());
}

generic::operator unsigned long() const 
{
	std::stringstream s(value_);
	unsigned long v;
	s>>v;
#ifdef __DEBUG__
	std::cout << ">>STRING <" << value_.c_str() << "> TO " << v << std::endl;
#endif
	return v;
}