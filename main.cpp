#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace std;

int main(){
    ifstream inFile("000001.XSHE.csv",ios::in);
    string lineStr;
    stringstream ss;
    vector<shared_ptr<map<string, double>>> data;
    vector<string> name;
    getline(inFile, lineStr);

    //get the title
    ss.str(lineStr);
    string s;
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
        cout<<lineStr<<endl;
        shared_ptr<map<string,double>> lineData = make_shared<map<string,double>>();
        ss.str(lineStr);
        for(int i=-1;i<name.size();i++)
        {
            if(i>-1)
            {
                double num;
                ss >> num;
                (*lineData)[name[i]]=num;
            }else {
                ss>>s;
            }
        }
        ss.clear();
        data.push_back(lineData);
    }
    
    for(int i=0;i<name.size();i++)
    {
        cout<< name[i]<<": "<<(*data[10])[name[i]]<<endl;
    }
    return 0;
 }
