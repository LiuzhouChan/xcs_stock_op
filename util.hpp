
#ifndef XCS_UTIL_HPP
#define XCS_UTIL_HPP
#include <memory>
#include <map>
#include <vector>

std::shared_ptr<std::vector<std::shared_ptr<std::map<std::string, double>>>> getData(const std::string& filePath="000001.XSHE.csv");
#endif // !XCS_UTIL_HPP
