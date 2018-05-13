
#ifndef XCS_RANDOM_HPP 
#define XCS_RANDOM_HPP

#include "config_magr2.hpp"

class xcs_random {
 private:
	//!  \var unsigned long seed to initialize the random number generator
	static unsigned long seed;
 public:
	//! name of the class that implements the random number generator.
	static std::string class_name() { return std::string("xcs_random"); };
	
	//! tag used to access the configuration file
	static std::string tag_name() { return std::string("random"); };

	//! set the seed for random number generation through the configuration manager
	static void set_seed(config_mgr2&);

	//! set the seed from random number generation
	static void set_seed(long seed);

	//! returns a random number in the real interval [0,1).
	static double random();

	//! returns a random number from a Gaussian distribution 
	static double nrandom();

	//! returns a random sign
	static int sign();

	/*! \brief returns an integer random number between 0 and limit-1.
	 *  \param limit the upper boundary for number generation
	 */
	static unsigned int dice(unsigned int limit);

	//! Saves the state of the random number generator to an output stream.
	/*! 
	 * It currently does not perform any action.
	 */
	static void save_state(std::ostream& output) {};

	//! Restores the state of the random number generator from an input stream.
	/*! 
	 * It currently does not perform any action.
	 */
	static void restore_state(std::istream& input) {};
};

#endif // !XCS_RANDOM_HPP 
