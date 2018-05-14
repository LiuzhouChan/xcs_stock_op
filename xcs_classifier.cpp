
#include "xcs_classifier.hpp"

unsigned long		xcs_classifier::id_count = 0;	
unsigned int		xcs_classifier::output_precision = 5;	


//! class constructor
xcs_classifier::xcs_classifier()
{
	set_initial_values();	
}

//! copy constructor
xcs_classifier::xcs_classifier(const xcs_classifier& classifier)
{
	set_initial_values();	
	condition = classifier.condition;
	action = classifier.action;
	prediction = classifier.prediction;		//!< prediction 
	error = classifier.error;			//!< prediction error
	fitness = classifier.fitness;			//!< classifier fitness
	actionset_size = classifier.actionset_size;	//!< estimate of the size of the action set [A]
	
	experience = classifier.experience;		//!< classifier experience, i.e., the number of times that the classifier has een updated
	numerosity = classifier.numerosity;		//!< classifier numerosity, i.e., the number of micro classifiers
	time_stamp = classifier.time_stamp;		//!< time of the last genetic algorithm application
}

//! copy constructor
xcs_classifier::xcs_classifier(xcs_classifier& classifier)
{
	set_initial_values();	
	condition = classifier.condition;
	action = classifier.action;
	prediction = classifier.prediction;		//!< prediction 
	error = classifier.error;			//!< prediction error
	fitness = classifier.fitness;			//!< classifier fitness
	actionset_size = classifier.actionset_size;	//!< estimate of the size of the action set [A]
	
	experience = classifier.experience;		//!< classifier experience, i.e., the number of times that the classifier has een updated
	numerosity = classifier.numerosity;		//!< classifier numerosity, i.e., the number of micro classifiers
	time_stamp = classifier.time_stamp;		//!< time of the last genetic algorithm application
}


bool operator==(const xcs_classifier& cl1, const xcs_classifier& cl2)
{
	return ((cl1.condition == cl2.condition) && (cl1.action == cl2.action));
}

bool operator!=(const xcs_classifier& cl1, const xcs_classifier& cl2)
{
	return !(cl1==cl2);
}

bool operator<(const xcs_classifier& cl1, const xcs_classifier& cl2)
{
	return ( (cl1.condition < cl2.condition) ||  
		 ((cl1.condition == cl2.condition) && (cl1.action < cl2.action)) );
}

std::ostream& operator<<(std::ostream& os,const xcs_classifier& cs)
{
	os << cs.id() << '\t';
	os << cs.condition << " : " << cs.action << '\t';
	os.setf(std::ios::scientific);
	os.precision(xcs_classifier::output_precision);
	os << cs.prediction << '\t';
  	os.precision(xcs_classifier::output_precision);
  	os << cs.error << '\t';
  	os.precision(xcs_classifier::output_precision);
  	os << cs.fitness << '\t';
  	os.precision(xcs_classifier::output_precision);
  	os << cs.actionset_size << '\t';
  	os.precision(xcs_classifier::output_precision);
  	os << cs.experience << '\t';
  	os.precision(xcs_classifier::output_precision);
  	os << cs.numerosity << ' ';
	return (os);
}


//! read the classifier from an input stream
std::istream& operator>>(std::istream& is, xcs_classifier& cs)
{
	char sep;
	if (!(is>>cs.identifier))
		return (is);

	if (!(is>>cs.condition))
		return (is);
		
	if (!(is>>sep))
		return (is);
		
	if (!(is>>cs.action))
		return (is);
		
	if (!(is>>cs.prediction))
		return (is);
		
	if (!(is >> cs.error))
		return (is);

	if (!(is >> cs.fitness))
		return (is);

	if (!(is >> cs.actionset_size))
		return (is);

	if (!(is >> cs.experience))
		return (is);

	is >> cs.numerosity;
	return (is);
}

//! pretty print the classifier to an output stream
void xcs_classifier::print(std::ostream& output)
const
{
	output << identifier << "\t";
	condition.print(output);
	output << " : " << action << "\t";
	output.setf(std::ios::scientific);
	output.precision(xcs_classifier::output_precision);
	output << prediction << '\t';
  	output.precision(xcs_classifier::output_precision);
  	output << error << '\t';
  	output.precision(xcs_classifier::output_precision);
  	output << fitness << '\t';
  	output.precision(xcs_classifier::output_precision);
  	output << actionset_size << '\t';
  	output.precision(xcs_classifier::output_precision);
  	output << experience << '\t';
  	output.precision(xcs_classifier::output_precision);
  	output << numerosity << ' ';
}

bool xcs_classifier::match(const binary_inputs& detectors)
{
	return (condition.match(detectors));
}

void	
xcs_classifier::random()
{
	condition.random();
	action.random();
	set_initial_values();
}

void xcs_classifier::cover(const binary_inputs& detectors)
{
	condition.cover(detectors);
	action.random();
	set_initial_values();
}

void xcs_classifier::mutate(const float mutationProb, const binary_inputs& detectors) 
{
	condition.mutate(detectors,mutationProb);
	action.mutate(mutationProb);
}

void xcs_classifier::recombine(xcs_classifier& classifier)
{
	condition.recombine(classifier.condition);
	std::swap(action,classifier.action);
}


bool xcs_classifier::subsume(const xcs_classifier& classifier) const
{
	return ((action==classifier.action) && this->condition.is_more_general_than(classifier.condition));
};


inline void  xcs_classifier::set_initial_values()
{
	identifier = xcs_classifier::id_count++;
	numerosity = 1;
	time_stamp=0;
	experience=0;
	prediction=0;
	error=0;
	fitness=0;
	actionset_size=0;
}

//! assignment operator
xcs_classifier&  xcs_classifier::operator=(xcs_classifier& classifier)
{
	set_initial_values();	
	condition = classifier.condition;
	action = classifier.action;
	prediction = classifier.prediction;		//!< prediction 
	error = classifier.error;			//!< prediction error
	fitness = classifier.fitness;			//!< classifier fitness
	actionset_size = classifier.actionset_size;	//!< estimate of the size of the action set [A]
	
	experience = classifier.experience;		//!< classifier experience, i.e., the number of times that the classifier has een updated
	numerosity = classifier.numerosity;		//!< classifier numerosity, i.e., the number of micro classifiers
	time_stamp = classifier.time_stamp;		//!< time of the last genetic algorithm application
	return *this;
}

//! assignment operator for a constant value
xcs_classifier& xcs_classifier::operator=(const xcs_classifier& classifier)
{
	set_initial_values();	
	condition = classifier.condition;
	action = classifier.action;
	prediction = classifier.prediction;		//!< prediction 
	error = classifier.error;			//!< prediction error
	fitness = classifier.fitness;			//!< classifier fitness
	actionset_size = classifier.actionset_size;	//!< estimate of the size of the action set [A]
	
	experience = classifier.experience;		//!< classifier experience, i.e., the number of times that the classifier has een updated
	numerosity = classifier.numerosity;		//!< classifier numerosity, i.e., the number of micro classifiers
	return *this;
}
