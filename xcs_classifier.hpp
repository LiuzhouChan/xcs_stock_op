
#ifndef XCS_CLASSIFIER_HPP
#define XCS_CLASSIFIER_HPP

#include <string>

#include "binary_input.hpp"
#include "condition.hpp"
#include "config_magr2.hpp"
#include "binary_action.hpp"
class xcs_classifier
{
 public:
	//! name of the class that implements XCS classifiers
	/*!
	 * \fn string class_name() 
	 * \brief name of the class that implements the environment
	 * 
	 * This function returns the name of the class. 
	 * Every class must implement this method that is used to 
	 * trace errors.
	 */
	std::string class_name() const { return std::string("xcs_classifier"); };

	//! class tag used in the configuration file; currently not used 
	std::string tag_name() const { return std::string("classifier"); };

 public:
	//! class constructor
	xcs_classifier(config_mgr2& config);

	//! class constructor
	xcs_classifier(const xcs_classifier&);

	//! class constructor
	xcs_classifier(xcs_classifier&);

	//! class destructor
	~xcs_classifier() {};

	//! equality operator
	bool operator==(const xcs_classifier& classifier)
	{
		return ((condition==classifier.condition) && (action==classifier.action));
	};

	//! inequality operator
	bool operator!=(const xcs_classifier& classifier)
	{
		return ((action!=classifier.action) || (condition!=classifier.condition));
	};

	//! less than operator
	bool operator<(const xcs_classifier& classifier)
	{
		return ((condition<classifier.condition) ||
		        ((condition==classifier.condition) && (action<classifier.action)));
	};

	//! equality operator used for sorting classifiers according to the identifier
	bool operator==(const unsigned long& id)
	{
		return (identifier==id);
	};

	//! less than operator used for sorting classifiers according to the identifier
	bool operator<(const unsigned long& id)
	{
		return (identifier<id);
	};

	friend bool operator==(const xcs_classifier&, const xcs_classifier&);
	friend bool operator<(const xcs_classifier&, const xcs_classifier&);
	friend bool operator!=(const xcs_classifier&, const xcs_classifier&);

	//! assignment operator
	xcs_classifier& operator=(xcs_classifier&); 

	//! assignment operator for a constant value
	xcs_classifier& operator=(const xcs_classifier&);

	//! return the label associated with the element in position fld returned by the stream operator
	std::string	stream_field(unsigned long fld) {};

	//! write the classifier to an output stream
	friend std::ostream& operator <<(std::ostream&, const xcs_classifier&);

	//! read the classifier from an input stream
	friend std::istream& operator >>(std::istream&, xcs_classifier&);

	//! return the label associated with the element in position fld returned by the stream operator
	std::string	print_field(unsigned long fld) {};

	//! pretty print the classifier on the output stream named "output"
	void print(std::ostream& output) const;

	//! save the state of the classifier class to an output stream
	static void	save_state(std::ostream& output) { output << id_count << std::endl;};

	//! restore the state of the classifier class from an input stream
	static void	restore_state(std::istream& input) {input >> id_count;};

	//! generate a random classifier
	void	random();			

	//!
	void	cover(const binary_inputs& input);
	//!
	bool	match(const binary_inputs& detectors);

	//! mutate the classifier according to the mutation probability "mu"
	void	mutate(const float mutationProb, const binary_inputs&); 

	//! apply crossover between this classifier and another one
	void	recombine(xcs_classifier& classifier);
	
	//! return true if this classifier subsumes the classifier "cs"
	bool	subsume(const xcs_classifier& cs) const;

	//! return the classifier id
	unsigned long	id() const {return identifier;};		
	
	//! generate the unique identifier (used when inserting the classifier in the population)
	void generate_id() {identifier = ++xcs_classifier::id_count;};	// 

 private:
	//!  set the classifier parameters to default values
	inline void set_initial_values();	

 public:
	bool is_more_general_than(xcs_classifier cl) const { return condition.is_more_general_than(cl.condition); };

 private:
	static 	unsigned long	id_count;		//!< global counter used to generate classifier identifiers
	static unsigned int	output_precision;	//!< precision of the output (useless)

 public:
	unsigned long		identifier;		//!< classifier identifier; it is unique for each classifier

	ternary_condition	condition;		//!< classifier condition
	binary_action		action;			//!< classifier action

	double			prediction;		// //!< prediction 
	double			error;			// //!< prediction error
	double			fitness;		// //!< classifier fitness
	double			actionset_size;		// //!< estimate of the size of the action set [A]
	
	unsigned long		experience;		// //!< classifier experience, i.e., the number of times that the classifier has een updated
	unsigned long		numerosity;		// //!< classifier numerosity, i.e., the number of micro classifiers
	unsigned long		time_stamp;		// //!< time of the last genetic algorithm application
};

#endif