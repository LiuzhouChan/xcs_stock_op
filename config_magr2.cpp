
#include <fstream>
#include <iostream>

#include "config_magr2.hpp"


config_mgr2::config_mgr2(std::string const& extension) 
{
	//! save file extension;
	file_extension = extension;

	//! open the configuration file
	std::string configFile =extension;
	std::ifstream file(configFile.c_str());

	if (!file.good())
	{
		std::string msg = "configuration file <" + configFile + "> not opened";
		error(class_name(),"scan", msg, 1);
	}
	
	//! clear the vector of tags
	tags.clear();
	
  	std::string line;
	std::string input_line;
	
  	std::string name;
  	std::string value;
  	std::string current_section;
  	std::string section_end;
  	
	int posEqual;
  	while (std::getline(file,input_line)) 
	{
		//! clear the line from leading and trailing spaces
		line=trim(input_line);
		
    		//! does not consider empty lines
    		if (!line.length()) continue;

		//! does not consider comments
		if (line[0] == '#') continue;
		if (line[0] == ';') continue;
	
		//! check for the start/end of a section
		if (input_line[0] == '<') 
		{

			//! check if is an end
			if (input_line[1]=='/')
			{
				//! end of a section
				section_end=trim(line.substr(2,line.find('>')-2));

				//cout << "*** " << current_section << endl;
				//cout << "*** " << section_end << endl;

				if (section_end!=current_section)
				{
					std::string msg = "section <"+current_section+"> ends with </"+section_end+">";
					error(class_name(),"constructor", msg, 1);
				} else {
					//! clear current section
					current_section = "";
				}

			} else {

				//! start of a new section
				if (current_section!="")
				{
					error(class_name(),"constructor", "nested section in <"+current_section+">", 1);
				}

				//! begin of a section
				current_section=trim(line.substr(1,line.find('>')-1));
				tags.push_back(current_section);

				//! check for '/' symbols in tags
				if (current_section.find('/')!=std::string::npos)
				{
					std::string msg = "symbol '/' not allowed in tag " + section_end;
					error(class_name(),"constructor", msg, 1);
				}
			}
			continue;
		}

		posEqual=line.find('=');
		
		if (posEqual==std::string::npos)
		{
			std::string msg = "configuration entry <" + line + "> not a comment, not a tag, not an assignment";
			error(class_name(),"constructor", msg, 1);
		}
			
		name  = trim(line.substr(0,posEqual));
		value = trim(line.substr(posEqual+1));
					content_[current_section+'/'+name]=generic(value);
	}
}

void config_mgr2::save(std::ostream &output) const
{
	for(auto tag = tags.begin(); tag!=tags.end(); tag++)
	{
		std::string current_tag = *tag;
		
		output << "<" << current_tag << ">" << std::endl;
		
		for(auto i=content_.begin();i!=content_.end();i++)
		{
			std::string s = i->first;				
			
			std::string t = s.substr(0,s.find('/'));	//! tag in list
			std::string p = s.substr(s.find('/')+1);		//! associated generic

			generic v = i->second;			//! associated value
			
			if (current_tag == t)
			{
				output << "\t" << p << " = " << v << std::endl;
			}
			
		}
		
		output << "</" << current_tag << ">" << std::endl;
	}
}

generic const& config_mgr2::Value(std::string const& section, std::string const& entry) const
{

	auto ci = content_.find(section + '/' + entry);
	if (ci == content_.end()) throw std::string(entry).c_str();
	return ci->second;
}

generic const& config_mgr2::Value(std::string const& section, std::string const& entry, double value)
{
	try {
		return Value(section, entry);
	} catch(const char*) {
		return content_.insert(std::make_pair(section+'/'+entry, generic(value))).first->second;
	}
}

generic const& config_mgr2::Value(std::string const& section, std::string const& entry, std::string const& value)
{
	try {
		return Value(section, entry);
	} catch(const char*) {
		return content_.insert(std::make_pair(section+'/'+entry, generic(value))).first->second;
	}
}
