
#include "xcs_random.hpp"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>


using namespace std;

//! \var unsigned long xcs_random::seed is seed for random number generator.
unsigned long	xcs_random::seed = 0;

/*! 
 * \fn void xcs_random::set_seed(xcs_config_mgr& xcs_config)
 * \param xcs_config represents the configuration manager currently in use.
 *
 * \brief set the seed for random number generation through the configuration manager.
 */
void xcs_random::set_seed(config_mgr2& xcs_config)
{

	//! look for the init section in the configuration file
	if (!xcs_config.exist(tag_name()))
	{
		error(class_name(), "constructor", "section <" + tag_name() + "> not found", 1);	
	}
	
	xcs_config.save(cerr);
	
	try {
		seed = xcs_config.Value(tag_name(), "seed");
	} catch (const char *attribute) {
		string msg = "attribute \'" + string(attribute) + "\' not found in <" + tag_name() + ">";
		error(class_name(), "constructor", msg, 1);
	}

	if (xcs_random::seed!=0)
	{
		xcs_random::set_seed(xcs_random::seed);
	} else {
		//! use clock to set the seed
		srand48(time(NULL));
	}
}

/*! 
 * \fn float xcs_random::random()
 *
 * \brief generate a real random number between 0 and 1.
 */
double xcs_random::random()
{
	return (double) drand48();
}

void xcs_random::set_seed(long new_seed)
{
	xcs_random::seed = new_seed;
 	srand48(xcs_random::seed);
}

/*! 
 * \fn unsigned int xcs_random::dice(const unsigned int limit)
 *
 * \brief generate an integer random number between 0 and limit - 1.
 */
unsigned int xcs_random::dice(const unsigned int limit)
{
	return ((unsigned int)((xcs_random::random()*float(limit))));
}

/*!
 * \fn double nrandom()
 *
 * \brief returns a floating-point random number generated according to a normal distribution with mean 0 and standard deviation 1
 */

double xcs_random::nrandom()
{
	double x1, x2, w;
	static bool	flag_have_number=false;		//! true if a number has been already generated
	static double	generated_number;		//! number generated at previous step

	if(flag_have_number)
	{
		flag_have_number = false; 
		return generated_number;
	} else {
		do
		{
			x1 = 2.0 * xcs_random::random() - 1.0;
			x2 = 2.0 * xcs_random::random() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );
   	 
		w = sqrt( (-2.0 * log( w ) ) / w );
		generated_number = x1 * w;
		flag_have_number = true;
		return x2 * w;
	}
  
}

//! returns a random sign
int xcs_random::sign()
{
	if (xcs_random::random()<0.5)
		return -1;
	else
		return 1;
}
