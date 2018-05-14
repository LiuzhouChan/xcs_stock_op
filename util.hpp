
#ifndef XCS_UTIL_HPP
#define XCS_UTIL_HPP
#include <memory>
#include <map>
#include <vector>
#include <ctime>
#include <sys/times.h>
#include <iomanip>

std::shared_ptr<std::vector<std::shared_ptr<std::map<std::string, double>>>> getData(const std::string& filePath="000001.XSHE.csv");


void error(std::string name, std::string method, std::string message, unsigned int exit_code);

std::string trim(std::string const& source, char const* delims = " \t\r\n");

std::string long2binary(const unsigned long, unsigned long);

unsigned long binary2long(const std::string &binary);

void set_flag(std::string set, bool& var);


class timer {
 private:
	 unsigned long ti;	//! init time
	 unsigned long tf;	//! stop time

 public:
	 timer() {
		struct tms reading;
		times(&reading);
		ti = reading.tms_utime;
	 };

	 ~timer() {};
	 void start()
	 {	 
		struct tms reading;
		times(&reading);
		ti = reading.tms_utime;
	 }

	 double time() const
	 {
		struct tms reading;
		times(&reading);
		return double(reading.tms_utime - ti)/sysconf(_SC_CLK_TCK);
	 }

	 void stop()
	 {	 
		struct tms reading;
		times(&reading);
		tf = reading.tms_utime;
	 }

	 double elapsed() const
	 {
		return double(tf - ti)/sysconf(_SC_CLK_TCK);
	 }

	 unsigned long initial() const { return ti; };
	 unsigned long final() const { return tf; };
};

#endif // !XCS_UTIL_HPP
