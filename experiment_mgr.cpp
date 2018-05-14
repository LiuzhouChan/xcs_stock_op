

#include "experiment_mgr.hpp"
#include "util.hpp"
#include "xcs_classifier.hpp"
#include "xcs_classifier_system.hpp"
#include <fstream>

using namespace std;

extern xcs_classifier_system * XCS;
const unsigned long MSGSTR = 256;
extern stock_env *Environment;

experiment_mgr::experiment_mgr(config_mgr2 &xcs_config)
{
	string str_test_environment;		//! string to read the setting for the test environment (on/off)
	string str_save_trace;			//! string to read the setting for trace (on/off)
	string str_save_agent_state;		//! string to read the setting for saving the agent state (on/off)
	string str_save_agent_report;		//! string to read the setting for saving the agent report (on/off)
	string str_save_experiment_state;	//! string to read the setting for trace (on/off)
	string str_trace_time;			//! string to read the setting for time trace (on/off)
	string str_compact_mode;		//! string to read the setting for compact mode (on/off)
	string str_save_learning;		//! string to read the setting for save learning mode (on/off)

	if (!xcs_config.exist(tag_name()))
	{
		error(class_name(), "constructor", "section <" + tag_name() + "> not found", 1);	
	}
	
	try {
		first_experiment = xcs_config.Value(tag_name(), "first experiment");
		no_experiments = xcs_config.Value(tag_name(), "number of experiments");
		first_learning_problem = xcs_config.Value(tag_name(), "first problem");
		no_learning_problems = xcs_config.Value(tag_name(), "number of learning problems");
		no_condensation_problems = xcs_config.Value(tag_name(), "number of condensation problems");
		no_test_problems = xcs_config.Value(tag_name(), "number of test problems");
		str_test_environment = (string) xcs_config.Value(tag_name(), "test environment");
		str_save_learning = (string) xcs_config.Value(tag_name(), "save learning problems");
		str_save_trace = (string) xcs_config.Value(tag_name(), "trace experiments");
		str_save_agent_state = (string) xcs_config.Value(tag_name(), "save population state");
		str_save_agent_report = (string) xcs_config.Value(tag_name(), "save population report");
		str_save_experiment_state = (string) xcs_config.Value(tag_name(), "save experiment state");
		save_stats_every = xcs_config.Value(tag_name(), "save statistics every");
		save_interval = xcs_config.Value(tag_name(), "save state every");
		str_trace_time = (string) xcs_config.Value(tag_name(), "trace time");
		str_compact_mode = (string) xcs_config.Value(tag_name(), "compact mode", "off");
	} catch (const char *attribute) {
		string msg = "attribute \'" + string(attribute) + "\' not found in <" + tag_name() + ">";
		error(class_name(), "constructor", msg, 1);
	}
	no_max_steps = xcs_config.Value(tag_name(), "maximum number of steps", 500);

	extension = xcs_config.extension();
	if (save_interval==0)
	{
		save_interval=1000000000;
	}

	set_flag(string(str_test_environment), flag_test_environment);
	set_flag(string(str_save_trace), flag_trace);
	set_flag(string(str_save_agent_state), flag_save_agent_state);
	set_flag(string(str_save_agent_report), flag_save_agent_report);
	set_flag(string(str_save_experiment_state), flag_save_state);
	set_flag(string(str_trace_time), flag_trace_time);
	set_flag(string(str_compact_mode), flag_compact_mode);

	//! when in compact mode, the trace is disabled
	if (flag_compact_mode)
		flag_trace = false;

	current_experiment = -1;	//! to check whether the method reset_experiments is called
	current_problem = -1;		//! to check whether the method reset_problems is called
}

void experiment_mgr::perform_experiments()
{
	char			sysCall[MSGSTR];			//! string used to perform system calls.
	ofstream		file_statistics;			//! files that contains the whole experiment statitics. One line for each problem performed.
	ofstream		file_trace;					//! files that contains the trace information about the experiment
	double			reward_sum = 0;				//! sum of rewards gained while solving the problem
	long			problem_steps = 0;			//! number of steps needed to solve the problem

	timer			timer_overall;				//! measure the CPU time for the all the experiments
	timer			timer_experiment;			//! measure the CPU time for one experiment
	timer			timer_problem;				//! measure the CPU time for one problem
	vector<double>	experiment_time;			//! time elapsed for each experiment
	vector<double>	problem_time;				//! time elapsed for problems
	double			average_problem_time;		//! average time for problems
	double			average_learning_time;		//! average time for learning
	double			average_testing_time;		//! average time for testing
	bool			flag_compact_stats_printed;	//! true if the compact line was just printed

	experiment_time.clear();
	problem_time.clear();
	timer_overall.start();
	
	//! performs all the experiments, one by one.
	for(current_experiment=first_experiment; current_experiment < (first_experiment+no_experiments); current_experiment++)
	{

		//! true if condensation is active
		bool flag_condensation = false;

		//! init XCS for the current experiment
		XCS->begin_experiment();
		
		//! the first problem is always solved in exploration
		bool flag_exploration = true;

		
		//! init the file for statistics
		char	file_statisticsName[MSGSTR];
		if (!flag_compact_mode)
			sprintf(file_statisticsName, "statistics.%s-%ld", extension.c_str(), current_experiment);
		else 
			sprintf(file_statisticsName, "compact_stats.%s-%ld", extension.c_str(), current_experiment);
		
		/*! 
		 * if first_learning_problem is greater than 0 indicates that the experiment must be restored from file; 
		 * otherwise the experiment starts from scratch.
		 */
		if (first_learning_problem>0)
		{	
			//! restore from the current experiment
			cout << "\nRestarting Experiment " << current_experiment;
			cout << "... " << endl;

			//! experiments statistics will be appened to existing files
			sprintf(sysCall,"gunzip %s.gz", file_statisticsName);
			system(sysCall);
			file_statistics.open(file_statisticsName,ios::out|ios::app);

			//! restores the state of the current experiment
			flag_exploration = restore_state(current_experiment);	
		} else {
			//! init the statistics file for a new experiment
			file_statistics.open(file_statisticsName);
		};

		if (!file_statistics.good())
		{
			char errMsg[MSGSTR] = "";
			sprintf(errMsg,"Statistics file '%s' not open",file_statisticsName);
			error(class_name(),"StartSession",string(errMsg),1);
		}

		char 	file_traceName[MSGSTR];
		sprintf(file_traceName, "trace.%s-%ld", extension.c_str(), current_experiment);

		if (flag_trace)
		{	
			/*! 
			 * if first_learning_problem is greater than 0 indicates that the experiment must be restored from file; 
			 * otherwise the experiment starts from scratch.
			 */
			if (first_learning_problem>0)
			{	
				sprintf(sysCall,"gunzip %s.gz", file_traceName);
				system(sysCall);
				file_trace.open(file_traceName,ios::out|ios::app);
			}
			else
			{	//! create a new trace file
				file_trace.open(file_traceName);
			}
			if (!file_trace.good())
			{
				char errMsg[MSGSTR] = "";
				sprintf(errMsg,"Trace file '%s' not open",file_traceName);
				error(class_name(),"StartSession",string(errMsg),1);
			}
		}
		
		//! start timer for the experiment
		timer_experiment.start();
		average_problem_time = 0;


		current_no_test_problems = 0;
		flag_compact_stats_printed = false;

		compact_average_steps = 0;
		compact_average_reward_sum = 0;
		compact_average_size = 0;

		//! performs the learning problems one by one
		for(current_problem=first_learning_problem; 
			current_problem<first_learning_problem+2*(no_learning_problems+no_condensation_problems)+no_test_problems; 
			current_problem++)
		{
			if (flag_compact_mode && !flag_exploration)
			{
				if ((current_no_test_problems!=0) && (current_no_test_problems%save_stats_every==0))
				{
					file_statistics << current_experiment << '\t' << current_no_test_problems << '\t';
					file_statistics << compact_average_steps/save_stats_every << '\t';
					file_statistics << compact_average_reward_sum/save_stats_every << '\t';
					file_statistics << compact_average_size/save_stats_every << "\tTesting";
					file_statistics << endl;
					flag_compact_stats_printed = true;
					compact_average_steps = 0;
					compact_average_reward_sum = 0;
					compact_average_size = 0;
				}
			}

			/*! 
			 * write the number of experiment and problem to the files
			 */

			//! save information in the statistics file
			if (!flag_compact_mode)
				//file_statistics << current_experiment << '\t' << current_problem+((current_problem-first_learning_problem)) << '\t';
				file_statistics << current_experiment << '\t' << current_problem << '\t';

			//! if needed save information in the trace file
			if (flag_trace)
			{
				file_trace << current_experiment << "\t" << current_problem << '\t';
			}
			
			//! start timer for problem
			timer_problem.start();

			//! init XCS for the current problem
			XCS->begin_problem();

			//! determine whether condensation should be activated
			flag_condensation = 
				((no_condensation_problems>0) && 
				(current_problem>=first_learning_problem+2*no_learning_problems));

			//! if learning has ended, the problems are performed in testing mode
			if (current_problem>=(first_learning_problem+2*(no_learning_problems+no_condensation_problems)))	
			{
				flag_exploration = false;
			}
			
			//! init the environment for the current problem
			Environment->begin_problem(flag_exploration);

			reward_sum = 0;
			problem_steps = 0;
			do 
			{			
				//! XCS executes one step
				XCS->step(flag_exploration,flag_condensation);
				problem_steps++;
				
				//! sum up the reward received
				reward_sum = reward_sum + Environment->reward();
				
			} 
			while ((problem_steps<no_max_steps) && (!Environment->stop()));

			//! stops the timer for the problem
			timer_problem.stop();
			average_problem_time += timer_problem.elapsed();

			//! problem trace information is saved
			/*! by default the statistics file contain (for each line)
			 *  - experiment number
			 *  - problem number
			 *  - trace information from XCS (usually null)
			 *  - trace information from the environment
			 *  - "Learning/Testing" whether the problem has been solved in learning or testing mode
			 */

			if (flag_trace) 
			{
				//! save trace information
				XCS->trace(file_trace);
				Environment->trace(file_trace);
				if (flag_exploration)
					file_trace << "\t" << "Learning" << endl;
				else 
					file_trace << "\t" << "Testing" << endl;
			}

			//! XCS ends the current problem
			XCS->end_problem();
			
			//! the environment ends the current problem
			Environment->end_problem();
			
			//! computes the average to be saved when in compact mode
			//! (current_no_test_problems==0) || ((current_no_test_problems%save_stats_every)==0)))
			if (flag_compact_mode)
			{
				if (!flag_exploration)
				{
					compact_average_steps += problem_steps;
					compact_average_reward_sum += reward_sum;
					compact_average_size += XCS->size();
				}
			}

			//! problem statistics are saved
			/*! by default the statistics file contain (for each line)
			 *  - experiment number
			 *  - problem number
			 *  - number of problem steps
			 *  - total reward gained during the problem
			 *  - population size
			 *  - "Learning/Testing" whether the problem has been solved in learning or testing mode
			 */

			if (!flag_compact_mode)
			{
				file_statistics << problem_steps << '\t';
				file_statistics << reward_sum << '\t';
				file_statistics << XCS->size() << '\t';
				//! if the environment is single step, it saves the system_error
				if (Environment->single_step())
				{
					file_statistics << XCS->get_system_error() << "\t";
				}
				file_statistics << (flag_exploration ? "Learning" : "Testing") << endl;
			}
		
			//! it switches from exploration to exploitation and viceversa
			flag_exploration = !flag_exploration;

			if ((current_problem-first_learning_problem>0) && ((current_problem-first_learning_problem)%save_interval==0))
			{
				if (flag_save_state) 
					save_state((current_experiment), flag_exploration, current_problem);
				//if (flag_save_avf)
				//	save_avf((current_experiment), flag_exploration, current_problem);
			}

			if (!flag_exploration)
			{
				current_no_test_problems++;
				flag_compact_stats_printed = false;
			}

		} //!< end learning/testing problems

		//! stops the experimnt timer
		timer_experiment.stop();

		//! memorize the time used in this experiment
		experiment_time.push_back(timer_experiment.elapsed());
		problem_time.push_back(average_problem_time/(no_learning_problems+no_condensation_problems+no_test_problems));



		/*!
		 *
		 * Test the environment 
		 * 
		 * for each possible initial configuration of the environment
		 * XCS is applied until the problem's end
		 *
		 */

		if (Environment->allow_test() && flag_test_environment)
		{
			Environment->reset_problem();

			do 
			{
				XCS->begin_problem();

				//! when testing the environment
				flag_exploration = false;
				flag_condensation = false;
				///==============================================================================
				/*! 
				 * write the number of experiment and problem to the files
				 */
		
				//! save information in the statistics file
				file_statistics << current_experiment << '\t' << current_problem << '\t';

					//! if needed save information in the trace file
					if (flag_trace)
					{
						file_trace << current_experiment << "\t" << current_problem << '\t';
					}
			
					reward_sum = 0;
					problem_steps = 0;
					do 
					{
						//! if more than 5000 steps have been performed, the systems is forced to explore
						if (problem_steps>5000)
						{
							flag_exploration = true;
						}
			
						//! XCS executes one step
						XCS->step(flag_exploration,flag_condensation);
						problem_steps++;
				
						//! sum up the reward received
						reward_sum = reward_sum + Environment->reward();
				
					} 
					while (!Environment->stop());

					//! problem trace information is saved
					/*! by default the statistics file contain (for each line)
					 *  - experiment number
					 *  - problem number
					 *  - trace information from XCS (usually null)
					 *  - trace information from the environment
					 *  - "Learning/Testing" whether the problem has been solved in learning or testing mode
					 */

					if (flag_trace) 
					{
						//! save trace information
						XCS->trace(file_trace);
						Environment->trace(file_trace);
						if (flag_exploration)
							file_trace << "\t" << "Learning" << endl;
						else 
							file_trace << "\t" << "Solution" << endl;
					}

					//! XCS ends the current problem
					XCS->end_problem();
			
					//! the environment ends the current problem
					Environment->end_problem();
			
					//! problem statistics are saved
					/*! by default the statistics file contain (for each line)
					 *  - experiment number
					 *  - problem number
					 *  - number of problem steps
					 *  - total reward gained during the problem
					 *  - population size
					 *  - "Learning/Testing" whether the problem has been solved in learning or testing mode
					 */

					file_statistics << problem_steps << '\t';
					file_statistics << reward_sum << '\t';
					file_statistics << XCS->size() << '\t';
					//! if the environment is single step, it saves the system_error
					if (Environment->single_step())
					{
						file_statistics << XCS->get_system_error() << "\t";
					}
					file_statistics << (flag_exploration ? "Learning" : "Solution") << endl;
		
					///==============================================================================
					current_problem++;

				} while (Environment->next_problem());
		} 
		
		//! stop the timer for the whole session
		timer_overall.stop();

		//! XCS ends the experiment
		XCS->end_experiment();
		
		//! at the end of the experiment the file for statistics is closed and gzipped
		file_statistics.close();
		sprintf(sysCall, "gzip -f %s", file_statisticsName);
		system(sysCall);
	
		if (flag_trace>0)
		{
			file_trace.close();
			sprintf(sysCall, "gzip -f %s", file_traceName);
			system(sysCall);
		}

		//! save requested information about the experiment.
		if (flag_save_state) 
		{
			clog << "\t" << current_experiment+1 << "/" << first_experiment+no_experiments << "\t";
			clog << "saving the experiment state...";
			save_state(current_experiment,flag_exploration);
			clog << "\t\t\tok" << endl;
		}

		if (flag_save_agent_report) 
		{
			clog << "\t" << current_experiment+1 << "/" << first_experiment+no_experiments << "\t";
			clog << "saving the population report...";
			save_agent_report(current_experiment);
			clog << "\t\t\tok" << endl;
		}

		if (flag_save_agent_state) 
		{
			clog << "\t" << current_experiment+1 << "/" << first_experiment+no_experiments << "\t";
			clog << "saving the population state...";
			save_agent_state(current_experiment);
			clog << "\t\t\tok" << endl;
		}
/*
		if (flag_save_avf) 
		{
			clog << "\t" << current_experiment+1 << "/" << first_experiment+no_experiments << "\t";
			clog << "saving the action value function...";
			save_avf(current_experiment);
			clog << "\t\t\tok" << endl;
		}
 */
	}
	if (flag_trace_time)
	{
		//! init the file for statistics
		ofstream	file_report;
		char		file_report_name[MSGSTR];

		sprintf(file_report_name, "report.%s-%ld", extension.c_str(), current_experiment);

		file_report.open(file_report_name,ios::out|ios::app);
		if (!file_report.good())
		{
			char errMsg[MSGSTR] = "";
			sprintf(errMsg,"Report file '%s' not open",file_report_name);
			error(class_name(),"StartSession",string(errMsg),1);
		}

		file_report << "TOTAL ELAPSED TIME\t\t" << setprecision(2) << timer_overall.elapsed() << endl;
		file_report << "DETAILS FOR EXPERIMENTS" << endl;
		file_report << endl;
		file_report << " EXP.";
		file_report << "           ELAPSED IN EXP.";
	        file_report << "           AVG FOR PROB." << endl;

		for(unsigned long exp=first_experiment; exp < (first_experiment+no_experiments); exp++)
		{
			file_report << setw(5) << exp << "\t";

			file_report << setw(20) << experiment_time[exp-first_experiment] << "sec\t";
			file_report << setw(20) << problem_time[exp-first_experiment] << "sec\t"; 
			file_report << endl;
		}
		file_report.close();
	

	}
};

void	
experiment_mgr::print_save_options(ostream& output) 
const
{
	output << "\nEXPERIMENT MANAGER SAVE OPTIONS\n" << endl;
			
	output << "\tsave population report:\t\t";
	output << (flag_save_agent_report ? "yes" : "no") << endl;
	output << "\tsave population state:\t\t";
	output << (flag_save_agent_state ? "yes" : "no") << endl;
	output << "\tsave experiment state:\t";
	output << (flag_save_state ? "yes" : "no") << endl;
											
	output << endl;
};

void	
experiment_mgr::save_agent_report(const unsigned long expNo, const unsigned long problem_no) const
{
	ofstream	output;			//!
	char		fileName[MSGSTR];	//!
	char		sysCall[MSGSTR];	//! string for system call

	if (problem_no==0)
		sprintf(fileName, "population_report.%s-%d", extension.c_str(), (int) expNo);
	else 
		sprintf(fileName, "population_report.%s-%d-%ld", extension.c_str(), (int) expNo, problem_no);

	output.open(fileName);
	if (!output.good())
	{
		string msg;
		msg = "Population file " + string(fileName) + " not open";
		error(class_name(),"save_agent", msg,1);
	}
	XCS->save_population(output);

	sprintf(sysCall, "gzip -f %s", fileName);
	system(sysCall);
}

void	
experiment_mgr::save_agent_state(const unsigned long expNo, const unsigned long problem_no) const
{
	ofstream	output;			//!
	char		fileName[MSGSTR];	//!
	char		sysCall[MSGSTR];	//! string for system call

	if (problem_no==0)
		sprintf(fileName, "population.%s-%d", extension.c_str(), (int) expNo);
	else 
		sprintf(fileName, "population.%s-%d-%ld", extension.c_str(), (int) expNo, problem_no);

	output.open(fileName);
	if (!output.good())
	{
		string msg;
		msg = "Population state file " + string(fileName) + " not open";
		error(class_name(),"save_agent", msg,1);
	}
	XCS->save_population_state(output);

	sprintf(sysCall, "gzip -f %s", fileName);
	system(sysCall);
}

void	
experiment_mgr::save_state(const unsigned long expNo, const bool flag_exploration, unsigned long problem_no) const
{
	ofstream	outfile;
	char	fileName[MSGSTR];
	char	sysCall[MSGSTR];	// string for system calls

	if (problem_no==0)
		sprintf(fileName, "experiment.%s-%d", extension.c_str(), (int) expNo);
	else 
		sprintf(fileName, "experiment.%s-%d-%ld", extension.c_str(), (int) expNo, problem_no);

	outfile.open(fileName);
	if (!outfile.good())
	{
		char errMsg[MSGSTR] = "";
		sprintf(errMsg, "Experiment's state file '%s' not created", fileName);
		error(class_name(),"save_state",string(errMsg),1);
	}
	else
	{
		outfile << flag_exploration;
		outfile << endl;
		xcs_random::save_state(outfile);
		outfile << endl;
		Environment->save_state(outfile);
		outfile << endl;
		XCS->save_state(outfile);
		outfile << endl;

		outfile.close();
		
		sprintf(sysCall, "gzip -f %s", fileName);
		system(sysCall);
	}
}

bool
experiment_mgr::restore_state(const unsigned long expNo)
{
	cout << "Restoring system state ... ";
	ifstream	infile;
	char	fileName[MSGSTR] = "";
	char	sysCall[MSGSTR];	// string for system calls
	bool	flag_exploration;
	
	sprintf(fileName, "experiment.%s-%d", extension.c_str(), (int) expNo);
	sprintf(sysCall, "gunzip %s", fileName);
	system(sysCall);
	infile.open(fileName);

	if (!infile.good())
	{
		char errMsg[MSGSTR] = "";
		sprintf(errMsg,
				"Experiment's state file '%s' not open",
				fileName);
		error(class_name(),"restore_state",string(errMsg),1);
	}

	infile >> flag_exploration;
	xcs_random::restore_state(infile);
	Environment->restore_state(infile);	
	XCS->restore_state(infile);
	infile.close();

	sprintf(sysCall, "gzip -f %s", fileName);
	system(sysCall);
	cout << "Ok\n" << endl;
	return flag_exploration;
}
