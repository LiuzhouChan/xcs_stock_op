

#ifndef XCS_STOCK_ENV_HPP
#define XCS_STOCK_ENV_HPP
#include <sstream>
#include <string>
#include <cassert>

#include "environment_base.hpp"
#include "config_magr2.hpp"

class stock_env: public virtual environment_base
{
public: 

	std::string class_name() const { return std::string("stock_env"); };
	std::string tag_name() const { return std::string("environment::stock_env"); };

    stock_env(config_mgr2&);
    ~stock_env() = default;

    void begin_problem(const bool explore);
	void end_problem() {};

	void begin_experiment() {};
	void end_experiment() {};

	bool stop() const;
	
	void perform(const binary_action& action);

	void trace(std::ostream& output) const;

	bool allow_test() const {return true;};
	void reset_problem();
	bool next_problem();

	void reset_input();
	bool next_input();

	void save_state(std::ostream& output) const;
	void restore_state(std::istream& input);


    virtual double reward() const { assert(current_reward==stock_env::current_reward); return current_reward;};
	virtual binary_inputs state() const { return inputs; };
	virtual void print(std::ostream& output) const { output << current_state << "\t";};
private:
    inline void	set_state();
	
	static bool			init;			//!< true if the class has been inited through the configuration manager
	binary_inputs		inputs;			//!< current input configuration
	
	long 				states_number;		//! number of states in the environment
	long				state_bits;		//! number of bits to encode the states
	long				final_state;		//! final state in the environment

        //! current reward returned for the last action performed
	double		current_reward;
	//unsigned long	no_configurations;			// #configurazioni possibili

	//! \var current_configuration index of the current agent's input
	/*!
	 * it is used when scanning all the possible environment configurations with 
	 * the functions \fn reset_input and \fn next_input
	 * \sa reset_input
	 * \sa next_input
	 */
	
        //! current x position in the environment
	unsigned long 	current_state;
         	
        //! \var prob_slide specifies the probability that the agent can slip while it moves (Colombetti and Lanzi 1999)
	double		prob_slide; 
	
	//! path traces the path the agent followed during the problem
	std::string			path;
};

#endif // !XCS_STOCK_ENV_HPP