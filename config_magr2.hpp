
#ifndef XCS_CONFIG_MAGR2_HPP 
#define XCS_CONFIG_MAGR2_HPP

#include <string>
#include <map> 
#include <algorithm>

#include "generic.hpp"
#include "util.hpp"

class config_mgr2
{
    std::map<std::string, generic> content_;
    std::vector<std::string> tags;
    std::string file_extension;
public:
    //! name of the class
	std::string class_name() const { return std::string("xcs_config_mgr"); };

    config_mgr2(std::string const& extension);

    //! return the file extension that is used by the configuration manager
	std::string extension() const {return file_extension;};

    //! true if a section labeled tag has been found in the configuration file 
	bool exist(const std::string tag) const {return find(tags.begin(), tags.end(), tag)!=tags.end(); };

	//! save the current configuration to an output stream 
	void save(std::ostream&) const;
	
	generic const& Value(std::string const& section, std::string const& entry) const;
	generic const& Value(std::string const& section, std::string const& entry, double value);
	generic const& Value(std::string const& section, std::string const& entry, std::string const& value);

};

#endif // !XCS_CONFIG_MAGR2_HPP 