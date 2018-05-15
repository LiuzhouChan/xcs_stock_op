

#ifndef XCS_STOCK_ENV_HPP
#define XCS_STOCK_ENV_HPP
#include <sstream>
#include <string>
#include <cassert>
#include <spdlog/spdlog.h>
#include <memory>
#include <fstream>
#include <iostream>

#include "config_magr2.hpp"
#include "account.hpp"
#include "binary_action.hpp"
#include "binary_input.hpp"
#include "util.hpp"


double getValue(std::shared_ptr<std::vector<std::shared_ptr<std::map<std::string, double>>>>const &data_, int64_t pos, std::string const & key);

class stock_env
{
public: 

	std::string class_name() const { return std::string("stock_env"); };
	std::string tag_name() const { return std::string("environment::stock_env"); };

    stock_env(config_mgr2&,std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("stockenv"));

    ~stock_env() {};

    void begin_problem(const bool explore);
	void end_problem() { current_state_ = data_->size()-2;};

	void begin_experiment() {};
	void end_experiment() {};

    bool single_step() const {return true;};
	//! writes trace information on an output stream; it is called just before the end_problem method \sa end_problem
	void trace(std::ostream& output){output << account_path;}

	bool stop() const {return current_state_ == data_->size()-2;}
	
	void perform(const binary_action& action);


	bool allow_test() const {return true;};
	void reset_problem() { reset_input();};
	bool next_problem() { return next_input();};

	void reset_input()
    {
        current_state_ = 1; 
        set_input(current_state_);
        account_= account("000001");
        account_.addMoney(1000000);
    }
	bool next_input()
    {
        updateAccountPath(getValue(data_, current_state_,"close"));

        if(current_state_==data_->size()-2) return false;
        current_state_++;
        set_input(current_state_);
        return true;
    }

	void save_state(std::ostream& output) const;
	void restore_state(std::istream& input);


    virtual double reward() const { assert(current_reward==stock_env::current_reward); return current_reward;};
	virtual binary_inputs state() const { return inputs; };
	virtual void print(std::ostream& output) const { output << current_state_ << "\t";};
private:
    void set_input(int64_t pos); //the pos must bigger than 0
    inline void updateAccountPath(double price)
    {
        std::ostringstream PATH; 
        PATH<<account_.getMoney()+account_.getStockAmount()*price<<"\t";
        account_path += PATH.str();
    }
	
	static bool			init;			//!< true if the class has been inited through the configuration manager
	binary_inputs		inputs;			//!< current input configuration
	

	double		current_reward;

	account account_;

    int64_t current_state_;
    std::shared_ptr<std::vector<std::shared_ptr<std::map<std::string, double>>>> data_;
    std::shared_ptr<spdlog::logger> logger_;
    std::string account_path;
};

#endif // !XCS_STOCK_ENV_HPP