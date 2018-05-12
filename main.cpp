
#include "util.hpp"
#include <iostream>

using namespace std;

int main(){
    auto data=getData("000002.XSHE.csv");
    // cout<<data->size()<<endl;
    for(auto i=(*data)[0]->begin();i!= (*data)[0]->end();i++)
    {
        cout<<i->first<<": "<<i->second<<endl;
    }
    return 0;
 }
