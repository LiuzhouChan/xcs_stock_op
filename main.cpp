
#include "util.hpp"
#include "xcs_random.hpp"
#include "xcs_classifier.hpp"
#include "xcs_classifier_system.hpp"
#include "config_magr2.hpp"
#include "stock_env.hpp"
#include "experiment_mgr.hpp"

#include <iostream>
#include <cmath>

using namespace std;

stock_env *Environment;
xcs_classifier_system *XCS;
experiment_mgr *Session;

bool flag_verbose = false;

int main(){
    cout<<xcs_random::sign()<<endl;
    return 0;
 }
