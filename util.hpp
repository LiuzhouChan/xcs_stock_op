
#ifndef XCS_UTIL_HPP
#define XCS_UTIL_HPP
#include <memory>
#include <map>
#include <vector>

std::shared_ptr<std::vector<std::shared_ptr<std::map<std::string, double>>>> getData(const std::string& filePath="000001.XSHE.csv");


void error(std::string name, std::string method, std::string message, unsigned int exit_code);

std::string trim(std::string const& source, char const* delims = " \t\r\n");

std::string long2binary(const unsigned long, unsigned long);

unsigned long binary2long(const std::string &binary);

#endif // !XCS_UTIL_HPP
