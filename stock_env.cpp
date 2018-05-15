

#include "stock_env.hpp"

bool stock_env::init = false;

stock_env::stock_env(config_mgr2& xcs_config,std::shared_ptr<spdlog::logger> logger):
 current_reward(0),account_(account("000001")),current_state_(0), logger_(logger),account_path(std::string(""))
{
    if(!stock_env::init)
    {
        if(!xcs_config.exist(tag_name()))
        {
            logger_->error("class:{} method:{} msg:{}",class_name(), "constructor", "section <" + tag_name() + "> not found");
            exit(1);
        }
        std::string filepath;
        try{
            filepath =  (std::string)xcs_config.Value(tag_name(),"stock data file");
            data_ = getData(filepath);
        }
        catch (const char *attribute){
            std::string msg = "attribute \'" + std::string(attribute) + "\' not found in <" + tag_name() + ">";
            logger_->error("class:{} method:{} msg:{}",class_name(), "constructor", msg);
            exit(1);
        }
    }
    
    stock_env::init = true;
}

void stock_env::begin_problem(const bool explore)
{
    account_.addMoney(100000);
    updateAccountPath(0);
    current_state_ = 1;
    set_input(current_state_);
}


void stock_env::set_input(int64_t pos) //the pos must bigger than 0
{
    std::string input("");
    if(getValue(data_,pos,"close") >= getValue(data_, pos-1,"close"))
    {
        input += "1";
    }
    else
    {
        input += "0";
    }

    if(getValue(data_,pos,"volume") >= getValue(data_, pos-1,"volume"))
    {
        input += "1";
    }
    else
    {
        input += "0";
    }

    if(getValue(data_,pos,"volume") >= getValue(data_, pos,"VMA20"))
    {
        input += "1";
    }
    else
    {
        input += "0";
    }

    if(getValue(data_,pos,"close") >= getValue(data_, pos,"PMA5"))
    {
        input += "1";
    }
    else
    {
        input += "0";
    }

    if(getValue(data_,pos,"close") >= getValue(data_, pos,"PMA10"))
    {
        input += "1";
    }
    else
    {
        input += "0";
    }
    inputs.set_string_value(input);
}

double getValue(std::shared_ptr<std::vector<std::shared_ptr<std::map<std::string, double>>>>const &data, int64_t pos, std::string const & key)
{
    return (*((*data)[pos]))[key];
}

void stock_env::perform(const binary_action& action)
{
    
}