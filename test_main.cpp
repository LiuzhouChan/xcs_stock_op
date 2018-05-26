
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

int main(){
    //  discovery component = off
    //  exploration strategy = RANDOM
    //  exploration mode = true
    spdlog::set_level(spdlog::level::off);  // if want to see the log, comment this line
    auto logger = spdlog::stdout_color_mt("test");
    string suffix = "/home/liu/毕业设计/code/confsys.stock.2";
    config_mgr2	xcs_config2(suffix);
    xcs_random::set_seed(xcs_config2);
    binary_action action(xcs_config2);
    ternary_condition condition(xcs_config2);
    stock_env environment(xcs_config2);
    for(int i=0; i< 20; i++)
    {
        spdlog::drop_all();
        environment.reset_input();
        xcs_classifier_system xcs(xcs_config2);

        xcs.setEnv(&environment);
        xcs.begin_experiment();
        xcs.begin_problem();
        environment.begin_problem(); 
        do{
            xcs.step(false,false);  // the first arg set to update the 
        }while(environment.next_input());
        string name="stock_account_info/stock_account_info_test"+to_string(i)+".txt";
        environment.trace(name);
        xcs.end_problem();
        environment.end_problem();
        spdlog::drop_all();
    }
    return 0;
 }
