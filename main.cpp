
#include "util.hpp"
#include "xcs_random.hpp"
#include "xcs_classifier.hpp"
#include "xcs_classifier_system.hpp"
#include "config_magr2.hpp"
#include "stock_env.hpp"
// #include "experiment_mgr.hpp"

#include <iostream>
#include <cmath>
#include <spdlog/spdlog.h>

using namespace std;



// experiment_mgr *Session;

bool flag_verbose = false;

int main(){
    auto logger = spdlog::stdout_color_mt("main");
    string suffix = "/home/liu/毕业设计/code/confsys.stock";
    config_mgr2	xcs_config2(suffix);
    xcs_random::set_seed(xcs_config2);
    binary_action action(xcs_config2);
    stock_env *environment= new stock_env(xcs_config2);

    ternary_condition condition(xcs_config2);
    xcs_classifier_system *xcs= new xcs_classifier_system(xcs_config2);
    xcs->setEnv(environment);
    xcs->begin_experiment();

    xcs->begin_problem();

    environment->begin_problem(); 

    do{
        xcs->step(true,false);
    }while(environment->next_input());

    environment->trace("stock_account_info.txt");

    xcs->end_problem();

    environment->end_problem();

    return 0;
 }
