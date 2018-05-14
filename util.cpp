
#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
//vector< <map<string, double> >
std::shared_ptr<std::vector<std::shared_ptr<std::map<std::string, double>>>> getData(const std::string& filePath)
{
    std::ifstream inFile(filePath, std::ios::in);
    std::string lineStr;
    std::stringstream ss;
    std::shared_ptr<std::vector<std::shared_ptr<std::map<std::string, double>>>> data = std::make_shared<std::vector<std::shared_ptr<std::map<std::string, double>>>>();
    std::vector<std::string> name;
    getline(inFile, lineStr);

    //get the title
    ss.str(lineStr);
    std::string s;
    while(ss>>s)
    {
        name.push_back(s);
    }
    ss.clear();

    // discard the fist 30 days
    for(int i=0;i<30;i++)
    {
        getline(inFile, lineStr);
    }
    
    while(getline(inFile, lineStr))
    {
        std::shared_ptr<std::map<std::string, double>> lineData = std::make_shared<std::map<std::string,double>>();
        ss.str(lineStr);
        ss>>s;
        for(int i = 0; i < name.size(); i++)
        {
            double num;
            ss >> num;
            (*lineData)[name[i]]=num;
        }
        ss.clear();
        data->push_back(lineData);
    }
    return data;
}



void error(std::string name, std::string method, std::string message, unsigned int exit_code)
{
	std::cerr << "\n\a***\tERROR";
	std::cerr <<   "\n***\tCLASS:  \t" << name;
	std::cerr <<   "\n***\tMETHOD: \t" << method;
	std::cerr <<   "\n***\tMESSAGE:\t" << message << "." << std::endl << std::endl;
	if (exit_code>0)
	{
		exit(exit_code);
	}
}


std::string trim(std::string const& source, char const* delims)
{
	std::string result(source);
	std::string::size_type index = result.find_last_not_of(delims);

	if(index != std::string::npos)
	{
		result.erase(++index);
	}

	index = result.find_first_not_of(delims);
	if(index != std::string::npos)
		result.erase(0, index);
	else
		result.erase();
	return result;
}

std::string long2binary(const unsigned long decimal, unsigned long size)
{
	std::string::size_type	bit;
	std::string			binary;
	unsigned long		base = 1;

	base <<= size;
	for (bit=0; bit<size; bit++)
	{
		base >>= 1;
		binary += '0'+((decimal & base)>0);
	};
	return binary;
}



unsigned long binary2long(const std::string &binary)
{
	unsigned long		power = 1;
	unsigned long		integer = 0;
	std::string::size_type	bit;

	for(bit=0; bit<binary.size(); bit++)
	{
		integer += (binary[binary.size()-bit-1]-'0') * power;
		power *= 2;
	}
	return integer;
}



void set_flag(std::string set, bool& var)
{
	if ((set=="on")||(set=="ON"))
		var = true;
	else if ((set=="off")||(set=="OFF"))
		var = false;
	else {
		error("xcs_utility", "set_flag", "value must be 'on' or 'off'", 1);
	}
}