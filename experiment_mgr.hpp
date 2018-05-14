
#ifndef XCS_EXPERIMENT_MGR_HPP
#define XCS_EXPERIMENT_MGR_HPP

#include "xcs_random.hpp"
#include "config_magr2.hpp"


class experiment_mgr
{

public:
	
	//! name of the class that implements the experiment manager
	std::string class_name() const { return std::string("experiment_mgr"); };

	//! tag used to access the configuration file
	std::string tag_name() const { return std::string("experiments"); };

	//! class constructor; it reads the class parameters through the configuration manager
	experiment_mgr(config_mgr2 &xcs_config);

	//! perform the experiments
	void perform_experiments();

	//! print the flags for save various experiment statistics
	void print_save_options(std::ostream& output) const;

	//! save the state of the experiment
	void save_state(const unsigned long, const bool, unsigned long problem_no=0) const;
	
	//! restore the state of the experiment
	bool restore_state(const unsigned long);

private:
	long	current_experiment;		//!< the experiment currently running
	long	first_experiment;		//!< number of the first experiment to run
	long	no_experiments;			//!< numbero of total experiments to run

	long	current_problem;		//!< the problem being currently executed
	long	first_learning_problem;		//!< first problem executed
	long	no_learning_problems;		//!< number of problems executed
	long	no_condensation_problems;	//!< number of problems executed in condensation
	long	no_test_problems;		//!< number of test problems executed at the end
	long	no_max_steps;		//!< maximum number of step per problem
	long	do_trace;


	long	current_no_test_problems;	//!< number of test problems solved so far


	bool	  	do_test_environment;		//!< test the environment for every possible start

	long		save_interval;			//! the experiment status is saved every "save_interval" problems

	bool		flag_trace;			//!< true if the experiment outputs on the trace file
	bool		flag_test_environment;		//!< true if the system will be tested on the whole environment
	bool		flag_save_state;		//!< true if the state of the system will be saved at the end of the experiment
	bool		flag_save_agent_state;		//!< true if the state of the agent must be saved when an experiment ends
	bool		flag_save_agent_report;		//!< true if the state of the agent must be saved when an experiment ends
	bool		flag_trace_time;		//!< true if execution time is traced

	std::string		extension;			//!< file extension for the experiment files
	
	bool		flag_compact_mode;		//!< false if the statistics of every problem is saved
	unsigned long	save_stats_every;		//!< number of problems on which the average is computed and the statistics is reported
	double		compact_average_steps;
	double		compact_average_reward_sum;
	double		compact_average_size;

	bool		flag_buffered_output;		//! true if the outputs files are written only once at the end of the experiment

 private:
	//! save the agent state for experiment \emph expNo
	void save_agent_report(const unsigned long expNo, const unsigned long problem_no=0) const;

	//! save the agent state for experiment \emph expNo
	void save_agent_state(const unsigned long expNo, const unsigned long problem_no=0) const;
	
	//! restore the agent state for experiment \emph expNo
	void restore_agent(const unsigned long expNo) const;

};


#endif // !XCS_EXPERIMENT_MGR_HPP