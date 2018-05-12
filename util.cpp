
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
