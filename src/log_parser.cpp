#include <regex>

#include "structs.hpp"




// ---------------- REGEX ---------------------- //

// ip address (?:^|[^\\d.])((?:(?:[1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])\\.){3}(?:[1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])(?![\\d.]))
// (.*?)
// timestamp (\[(.*?)\])
// (.*?)
// method + url + http version (\"(.*)\")
// (.*?)
// status code ([0-9]+)
// (.*?)
// bytes sent ([0-9]+)

//---------------ip address-----------------------------------------------------------------------------------------------------------timestamp----path/url/http-----status------bytes------

std::regex rgx("(?:^|;[^\\d.])((?:(?:[1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])\\.){3}(?:[1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])(?![\\d.])).*?\\[(.*?)\\].*?\\\"(.*)\\\".*?([0-9]+).*?([0-9]+)");

LogReader reader;


class LogParser{

	public:

		log_entry parseLine(std::string line){
			std::smatch matches;
			std::regex_search(line, matches, rgx);

			return log_entry {
				matches[1].str(),
				matches[2].str(),
				matches[3].str(),
				matches[4].str(),
				static_cast< unsigned short >( std::strtoul((matches[5].str()).c_str(), NULL, 0) ),
				static_cast<int>( std::strtoul((matches[6].str()).c_str(), NULL, 0) )
			};



		}
};


