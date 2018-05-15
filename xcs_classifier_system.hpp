
#ifndef XCS_CLASSIFIER_SYSTEM_HPP
#define XCS_CLASSIFIER_SYSTEM_HPP

#include <list>

#include "xcs_random.hpp"
#include "config_magr2.hpp"
#include "binary_action.hpp"

#include "binary_input.hpp"
#include "xcs_classifier.hpp"
#include "stock_env.hpp"
#include <spdlog/spdlog.h>
#include <memory>

//! covering strategy
typedef enum { 
 	COVERING_STANDARD,	//! perform covering based on the average prediction of [M] and [P]
	COVERING_ACTION_BASED	//! perform covering based on the number of actions in [M]
} t_covering_strategy;

//! deletion strategy
typedef enum { 
 	XCS_DELETE_RWS_SETBASED,			//! delete with RWS according to actionset size
 	XCS_DELETE_RWS_FITNESS,				//! delete with RWS according to actionset size and fitness
	XCS_DELETE_RANDOM,					//! random delete
	XCS_DELETE_RANDOM_WITH_ACCURACY,	//! random delete based on numerosity+accuracy enhancement
	XCS_DELETE_TSES,		//! delete with TS according to the maximization of error
	XCS_DELETE_TSE,			//! delete with TS according to the maximization of error with threshold
	XCS_DELETE_TSF,			//! delete with TS according to the minimization of fitness
	XCS_DELETE_TSFS,		//! delete with TS according to the minimization of fitness without threshold
	XCS_DELETE_TSSS			//! delete with TS according to the maximization of action set size
} t_delete_strategy;

//! selection strategy
typedef enum { 
	XCS_SELECT_RWS_FITNESS,		//! select offspring classifiers based on RWS and fitness
	XCS_SELECT_TS_FITNESS,		//! offspring selection based on TS and fitness maximization
	XCS_SELECT_TS_ERROR,		//! offspring selection based on TS and error minimization
} t_offspring_selection_strategy;

//! action selection strategy
typedef enum { 
	ACTSEL_SEMIUNIFORM, 	// Uniform con prob. biased deterministico
	ACTSEL_PROPORTIONAL, 	// Prob. proporzionale alla predizione
} t_action_selection;

//! population init strategy
typedef enum {
	INIT_RANDOM, 		// random population
	INIT_EMPTY, 		// empty population
	INIT_LOAD, 		// empty population
} t_classifier_set_init;

/*!
 * \brief implements the elements of the prediction array
 */
class t_system_prediction {
 public:
        double		payoff;		//! action payoff
        binary_action	action;		//! action
        double		sum;		//! fitness sum to normalize action payoff
	unsigned long 	n;		//! number of classifiers that advocate the action

	t_system_prediction()
	{
		payoff = 0;
		sum = 0;
		n = 0;
	};

	int operator==(const t_system_prediction& p2) const
	{	// due predizioni sono 'uguali' se si riferiscono alla stessa azione
		return (action == p2.action);
	}

	int operator==(const binary_action& act) const
	{	// due predizioni sono 'uguali' se si riferiscono alla stessa azione
		return (action == act);
	}
};

/*!
 * \class xcs_statistics
 * \brief collects various XCS statistics
 */
class xcs_statistics
{
	public:
		double	average_prediction;		//! average prediction in [P]
		double	average_fitness;		//! average fitness in [P]
		double	average_error;			//! average prediction error in [P]
		double	average_actionset_size;		//! average size of [A]
		double	average_experience;		//! average experience
		double	average_numerosity;		//! average numerosity
		double	average_time_stamp;		//! average time stampe
		double	average_no_updates;		//! average number of classifier updates
		double	system_error;			//! system error in [P]

		unsigned long no_macroclassifiers;	//! number of macroclassifiers in [P]
		unsigned long no_ga;			//! number of GA activations
		unsigned long no_cover;			//! number of covering activations 
		unsigned long no_subsumption;		//! number of subsumption activations

		//! class constructor; it invokes the reset method \sa reset
		xcs_statistics();

		//! reset all the collected statistics
		void reset();

		//! read the statistics from an output stream
		friend std::istream& operator>>(std::istream&, xcs_statistics&);

		//! write the statistics to an output stream
		friend std::ostream& operator<<(std::ostream&, const xcs_statistics&);
};		 

/*!
 * \class xcs_classifier_system
 * \brief implements the XCS classifier system
 */
class xcs_classifier_system
{
 public:
	//! name of the class that implements XCS
	std::string class_name() const {return std::string("xcs_classifier_system");};

	//! tag used to access the configuration file
	std::string tag_name() const {return std::string("classifier_system");};

	//! class constructor
	xcs_classifier_system(config_mgr2& xcs_config, std::shared_ptr<spdlog::logger> logger= spdlog::stdout_color_mt("xcs_classifier_system"));

	/**
	 * methods used from the experiment manager
	 */

	//@{

	//! defines what has to be done when a new experiment begins
	void	begin_experiment();

	//! defines what has to be done when the current experiment begins
	void	end_experiment();
	
	//! defines what has to be done when a new problem begins. 
	void	begin_problem();

	//! defines what must be done when the current problem ends
	void	end_problem();

	//! return the size of memory that is used during learning, i.e., the number of macro classifiers in [P]
	unsigned long size() const {return population.size();};

	//! return the current system error
	double get_system_error() const {return system_error;};

	//! writes trace information on an output stream; 
	/*! 
	 * it is called by the experiment manager just before the end_problem method 
	 * \sa end_problem
	 * \sa \class experiment_mgr
	 */
	void trace(std::ostream& output) const {};

	//!	return the statistics of the current experiment
	xcs_statistics statistics() const {return stats;};

	//!	restore XCS state from an input stream
	void	restore_state(std::istream& input);

	//!	save the experiment state to an output stream
	void	save_state(std::ostream& output);

	//!	save the population state to an output stream
	void	save_population_state(std::ostream& output);

	//!	save population
	void	save_population(std::ostream& ouput);

	//@}

 public:
	//================================================================================
	//
	//
	//	PUBLIC TYPES
	//
	//
	//================================================================================
	
	//! pointer to a classifier
	typedef xcs_classifier*				t_classifier_ptr;

	//! set of classifiers
	/*! 
	 * \type t_classifier_set
	 * \brief represent a set of classifiers
	 */
	typedef std::vector<xcs_classifier*>			t_classifier_set;

	//! index in set of classifiers
	/*! 
	 * \type t_set_iterator
	 * \brief represent an iterator over a set of classifiers
	 */
	typedef std::vector<xcs_classifier*>::iterator	t_set_iterator;
	typedef std::vector<xcs_classifier*>::const_iterator	t_set_const_iterator;

private:
	//================================================================================
	//
	//
	//	PERFORMANCE COMPONENT
	//
	//
	//================================================================================
	std::shared_ptr<spdlog::logger> logger_;			
	bool			init;				//! true, if the class was initialized
	stock_env		*environment;			//! link to the environment

	//! experiment parameters
	unsigned long		total_steps;			//! total number of steps 
	unsigned long		total_learning_steps;		//! total number of exploration steps
	unsigned long		total_time;			//! total time passed (should be the same as total steps);
	unsigned long		problem_steps;			//! total number of steps within the single problem
	double			total_reward;			//! total reward gained during the problem
	double			system_error;			//! difference between predicted payoff and reward received (it is used only in single step problems)

	//! [P] parameters
	unsigned long		max_population_size;		//! maximum number of micro classifiers
	unsigned long		population_size;		//! population size
	unsigned long		macro_size;			//! number of macroclassifiers
	t_classifier_set_init	population_init;		//! init strategy for [P]
	std::string			population_init_file;		//! file containing a population to be used to init [P]

	//! classifier parameters
	double			init_prediction;		//! initial prediction for newly created classifiers
	double			init_error; 			//! initial predition error for newly created classifiers
	double			init_fitness; 			//! initial fitness for newly created classifiers
	double			init_set_size;			//! initial set size for newly created classifiers
	unsigned long		init_no_updates;		//! initial number of updates for newly created classifiers

	//! covering strategy
	t_covering_strategy	covering_strategy;		//! strategy for create covering classifiers
	double			fraction_for_covering;		//! original covering parameter in Wilson's 1995 paper
	double	 		tetha_nma;			//! minimum number of actions in [M]

	//! action selection
	t_action_selection	action_selection_strategy;	//! strategy for action selection
	double			prob_random_action;		//! probability of random action

	///================================================================================
	///
	///
	///	REINFORCEMENT COMPONENT
	///
	///
	///================================================================================
						
	//! fitness computation
	double			epsilon_zero;			//! epsilon zero parameter, determines the threshold
	double			alpha; 				//! alpha parameter, determines the start of the decay
	double			vi;				//! vi parameter, determines the decay rate
	bool			use_exponential_fitness;	//! if true exponential fitness is used


	//================================================================================
	//
	//
	//	DISCOVERY COMPONENT
	//
	//
	//================================================================================
						
	//! GA parameters
	bool			flag_discovery_component;	//! true if the GA is on
	double			theta_ga;			//! threshold for GA activation
	double			prob_crossover;			//! probability to apply crossover
	double			prob_mutation;			//! probability to apply mutation
	bool			flag_ga_average_init;		//! if true offspring parameters are averages
	bool			flag_error_update_first;	//! if true, prediction error is updated first

	bool			flag_update_test;		//! true if update is performed during test

	bool			use_ga;				//! true if GA is on
	bool			use_crossover;			//! true if crosseover is used
	bool			use_mutation;			//! true if mutation is used

	//! subsumption deletion parameters
	bool			flag_ga_subsumption;		//! true if GA subsumption is on
	bool			flag_gaa_subsumption;		//! true if Butz GA subsumption is used
	bool			flag_as_subsumption;		//! true if AS subsumption is on
	double			theta_sub;			//! threshold for subsumption activation
	double	 		theta_as_sub;			//! threshold for subsumption activation
	bool			flag_cover_average_init;

	//! delete parameters
	t_delete_strategy	delete_strategy;		//! deletion strategy
	bool			flag_delete_with_accuracy;	//! deletion strategy: true, if T3 is used; false if T1 is used
	double			theta_del;			//! theta_del parameter for deletion
	double			delta_del;			//! delta parameter for deletion

	//================================================================================
	//
	//
	//	OTHER PRIVATE FUNCTIONS
	//
	//
	//================================================================================
						
	xcs_statistics stats;					//! classifier system statistics

 public:
	//================================================================================
	//
	//
	//	REINFORCEMENT COMPONENT
	//
	//
	//================================================================================
						
	double	learning_rate;					//! beta parameter
	double	discount_factor;				//! gamma parameter, the discount factor

	//================================================================================
	//
	//
	//	METHODS FOR THE DISCOVERY COMPONENT
	//
	//
	//================================================================================

 private:
	//! private methods for GA
	//@{
	bool	need_ga(t_classifier_set &action_set, const bool flag_explore);
	void	select_offspring(t_classifier_set&, t_set_iterator&, t_set_iterator&);
	//void	genetic_algorithm(t_classifier_set &action_set, const t_state& detectors, const bool flag_condensation);
	void	genetic_algorithm(t_classifier_set &action_set, const binary_inputs& detectors, const bool flag_condensation=false );
	//@}

 private:

	//! variables for [P], [M], [A], and [A]-1
	//@{
	t_classifier_set 				population;			//! population [P]
	t_classifier_set				match_set;			//! match set [M]
	t_classifier_set				action_set;			//! action set [A]
	t_classifier_set				previous_action_set;		//! action set at previous time step [A]-1
	//@}

	std::vector<double>					select;				//! vector for roulette wheel selection
	std::vector<double>					error;

	binary_inputs					previous_input;			//! input at t-1
	binary_inputs					current_input;			//! current input at time t
	std::vector<t_system_prediction>			prediction_array;		//! prediction array P(.) 
	std::vector<unsigned long>				available_actions;		//! actions in the prediction array that have a not null prediction, and thus are available for selection

	double		previous_reward;						//! reward received at previous time step


	//! methods for setting the parameters from the configuration file
	//@{
	void	set_init_strategy(std::string);				
	void	set_exploration_strategy(const char* explorationType);
	void	set_deletion_strategy(const char* deleteType);
	void	set_covering_strategy(const std::string strategy, double);			//! set the covering strategy, which can be "standard" or "nma"
	//@}

	//! methods for [P]
	//@{
	void	clear_population();				//! empty [P]
	void    init_classifier_set();				//! init [P] according to the selected strategy (i.e., empty or random)

	void	erase_population();				//! erase [P]
	void	insert_classifier(xcs_classifier& cs);		//! insert a classifier in [P]
	void	delete_classifier();				//! delete a classifier from [P]

	//! init the classifier parameters
	void	init_classifier(xcs_classifier& classifier, bool average=false);	
	//void	init_classifier(t_classifier& classifier, bool average);	
	//@}

	//! methods for covering
	//@{
	bool	need_covering_standard(t_classifier_set, const binary_inputs&);	//! true if covering is needed according to Wilson 1995

	void	covering(const binary_inputs& detectors);				//! covering
	bool	perform_covering(t_classifier_set&, const binary_inputs&);		//! perform covering in [M]

	bool	perform_standard_covering(t_classifier_set&, const binary_inputs&); //! perform covering according to Wilson 1995
	bool	need_standard_covering(t_classifier_set&, const binary_inputs&);	//! true if standard covering is needed

	bool	perform_nma_covering(t_classifier_set&, const binary_inputs&);	//! perform covering according to Butz and Wilson 2001

	//@}
	
	//!	build the prediction array P(.) from [M]
	void	build_prediction_array();

	//! 	print the prediction array P(.) to an output stream
	void	print_prediction_array(std::ostream&) const;

	//! select an action
	void	select_action(const t_action_selection, binary_action&);
	
	//!	build [A] based on the selected action a
	void	build_action_set(const binary_action&);

	//! methods for distributing the reinforcement among classifiers
	//@{
	void	update_set(const double, t_classifier_set&);
	void	update_fitness(t_classifier_set&);
	//@}

	//! true if classifier \emph first subsume classifier \emph second
	bool 	subsume(const xcs_classifier& first, const xcs_classifier& second);


 public:
	//@{
	/*! 
	 * find the classifiers that are AS subsumed in the current set
	 * \param set input classifiers
	 * \param set classifiers that are AS subsumed
	 */
	void	do_as_subsumption(t_classifier_set &set);

	//! Butz subsumption: check whether a classifier is subsumed by any classifier in [A] 
	void ga_a_subsume(t_classifier_set&, const xcs_classifier&, t_set_iterator&);

	//!	find the "most general classifier in the set
	//t_set_iterator find_most_general(t_classifier_set&) const;
	t_set_iterator find_most_general(t_classifier_set&) const;

	//! 	find the classifiers in the set that are subsumed by classifier
	void	find_as_subsumed(t_set_iterator, t_classifier_set&, t_classifier_set&) const;

	//!	perform AS subsumption, deleting the subsumed classifiers and increasing the numerosity of most general classifier
	void	as_subsume(t_set_iterator, t_classifier_set &set);

	//@}
 public:
	//!	a step of a problem
	void	step(const bool exploration_mode,const bool condensationMode);

	//!  build the match set [M]; it returns the number of microclassifiers that match the sensory configuration
	unsigned long	match(const binary_inputs& detectors);

 private:
	//! true if the problem is solved in exploration (Wilson 1995), i.e., XCS is in learning (Butz 2001)
	bool		exploration_mode;	
 public:
	//! total number of learning steps
	unsigned long	learning_time() const {return total_learning_steps;};
	
	//! total number of steps
	unsigned long	time() const {return total_steps;};

	//! total number of steps within the current problem
	unsigned long	problem_time() const {return problem_steps;};
 
	//! return true if the XCS is solving the problem in exploration
	bool	in_exploration() {return exploration_mode;};

	//! set the exploration mode: if mode is true problems will be solved in exploration
	void	exploration(bool mode) {exploration_mode = mode;};

	/*! 
	 * TRAIN/TEST FUNCTIONS
	 */
	void train(binary_inputs&, binary_action&, double) {};
	void test(binary_inputs&, binary_action&, double) {};

 private:
	bool	classifier_could_subsume(const xcs_classifier &classifier, double epsilon_zero, double theta_sub) const
		{ return ((classifier.experience>theta_sub) && (classifier.error<epsilon_zero)); };

 private:
	//! create the prediction array based on the action used
	void	create_prediction_array();

	//! clear the prediction array based
	void	init_prediction_array();

	//@{ \defgroup procedures to trace execution
	
	//! print a set of classifiers
	void print_set(t_classifier_set &set, std::ostream& output);

	//! check the consistency of various parameters in [P]
	void	check(std::string, std::ostream&);
	//@}
	
	bool comp_num(const xcs_classifier& cl1, const xcs_classifier& cl2) const {return (cl1.numerosity>cl2.numerosity);};

	class comp_numerosity {
  	  public:
	  int operator()(const xcs_classifier_system::t_classifier_ptr &first, const xcs_classifier_system::t_classifier_ptr &second)
	  {
	    return (first->numerosity>second->numerosity);
	  };
	};

	class comp_experience {
  	  public:
	  int operator()(const xcs_classifier_system::t_classifier_ptr &first, const xcs_classifier_system::t_classifier_ptr &second)
	  {
	    return (first->experience>second->experience);
	  };
	};

 private:
	//! configuration
	// const std::string	configuration[]; 

 private:
 	//! new random init procedure
	void init_population_random();

 	//! new random init procedure
	void init_population_load(std::string);

 private:
	//! new configuration parameters 2003/05/06

	bool			flag_use_mam;
	bool			flag_ga_tournament_selection;
	bool			flag_tournament_on_fitness;
	double 			tournament_size;

	//! select offspring classifier through tournament selection
	void			select_offspring_ts(t_classifier_set&, t_set_iterator&);
 private:
	/*!
	    selection strategies for deletion
         */
	//! random deletion
	t_set_iterator select_delete_random(t_classifier_set &set);

	//! roulette wheel
	t_set_iterator select_delete_rw(t_classifier_set &set);

	/*!
	    gradient descent
         */

 	//! true if gradient descent is used	
	bool	flag_use_gradient_descent;

 public:
	//! returns the specificity in a set
	double	specificity(const t_classifier_set &set) const;

	//! returns the average gradient in a set
	double	average_gradient(const t_classifier_set &set) const;
};


#endif // !XCS_CLASSIFIER_SYSTEM_HPP