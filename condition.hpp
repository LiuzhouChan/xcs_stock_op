
#ifndef XCS_CONDITION_HPP
#include <string>
#include "config_magr2.hpp"
#include "binary_input.hpp"

class ternary_condition
{
public:
	static const char	dont_care;			//! don't care symbol used in conditions

private:
	static bool		init;				//!< true if the class has been already inited through the configuration manager

	std::string			bitstring;						//!< condition string
	static unsigned long	no_bits;				//!< number of bits in condition
	static double		dont_care_prob;				//!< probability of having a don't care in a random condition
	static bool		flag_mutation_with_dontcare;	//!< true if #s are used in mutation
	static unsigned long	crossover_type;			//!< default crossover type (0=uniform, 1=one-point, 2=two-points)
	static unsigned long	mutation_type;			//!< default mutation type (1=niche mutation, 2=two-values, 3=pure or three values)

public:
	//! name of the class that implements the condition
	std::string class_name() const { return std::string("ternary_condition"); };
	
	//! tag used to access the configuration file
	std::string tag_name() const { return std::string("condition::ternary"); };

	//! return the condition as a string
	std::string string_value() const {return bitstring;};

	//! set the condition to a value represented as a string
	void set_string_value(std::string str) {bitstring = str;};

	//! return the condition size
	unsigned long size() const {return bitstring.size();};

	//
	//	class constructors
	//
	
	//! Constructor for the ternary condition class that read the class parameters through the configuration manager
	/*!
	 *  This is the first constructor that must be used. Otherwise an error is returned.
	 */
	ternary_condition(config_mgr2&);

	//! Default constructor for the ternary condition class
	/*!
	 *  If the class parameters have not yet initialized through the configuration manager, 
	 *  an error is returned and the method exists to shell. 
	 *  \sa ternary_condition(xcs_config_mgr&)
	 *  \sa xcs_config_mgr
	 */
	ternary_condition();


	~ternary_condition() = default;
	
	//	
	//	comparison operators
	//	
	

	bool operator< (const ternary_condition& cond) const;


	bool operator==(const ternary_condition& cond) const;

	bool operator!=(const ternary_condition& cond) const;

	//
	//	assignment operators
	//
	

	ternary_condition& operator=(ternary_condition& cond); 

	ternary_condition& operator=(const ternary_condition& cond);

	// 
	// 	match operator
	//
	
	//! return true if the condition matches the input configuration
	bool match(const binary_inputs& input) const;

	// 
	//	cover operator
	//
	
	//! set the condition to cover the input 
	void cover(const binary_inputs& input);

	//
	//	genetic operators
	//

	//! mutate the condition according to the mutation rate \emph mu
	void mutate(const double&);

	//! mutate the condition according to the mutation rate \emph mu; the mutating bits are set according to the current input
	void mutate(const binary_inputs &sens, const double& mu);

	//! recombine the condition according to the strategy specified by the method variable
	// \param method the crossover type to be used
	void recombine(ternary_condition&, unsigned long method=crossover_type);

	// 
	//    pretty print the condition
	//   
	
	//! pretty print the condition to the output stream "output".
	void print(std::ostream& output) const { output << bitstring; };

    friend std::ostream& operator<<(std::ostream& output, const ternary_condition& cond)
	{
		output << cond.string_value();
		return (output);
	};

	//! read the condition from an input stream 
	friend std::istream& operator>>(std::istream& input, ternary_condition& cond)
	{
		std::string	str;
		input >> str;
		cond.set_string_value(str);
		return (input);
	};

	//! true if the representation allow the use of GA subsumption
	virtual bool allow_ga_subsumption() {return true;};

	//! true if the representation allow the use of action set subsumption
	virtual bool allow_as_subsumption() {return true;};

	//! return true if the condition is more general than (i.e., subsumes) \emph cond
	bool is_more_general_than(const ternary_condition& cond) const;

	//! generate a random condition
	void random();

 private:
	/// single point crossover
	void	single_point_crossover(ternary_condition&);
 
	/// two point crossover
	void	two_point_crossover(ternary_condition&);
 
	/// uniform crossover
	void	uniform_crossover(ternary_condition&);

 public:
	//! generality
	double generality() const;

	//! specificity
	double specificity() const;
};

#endif // !XCS_CONDITIONBASE
