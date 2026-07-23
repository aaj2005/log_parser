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

		LogParser(std::vector<std::string> files) {
			reader = LogReader();
			for (std::string file : files){
				reader.open_file(file);
			}
		}
		~LogParser(){reader.close_all();}

		void parse_all(){
			std::vector<std::string> files = reader.get_paths();
			for (std::string file: files){
				std::string_view line = "";
				while (line != "EOF"){
					line = reader.getNextLine(file);
					log_entry entry = parseLine(line);

					
				} 
			}
		}

		

	private:
		// "192.168.1.5 - - [22/Jul/2026:12:01:00 +0000] \"GET /home HTTP/1.1\" 200 1024"
		log_entry parseLine(std::string_view line){
			std::smatch matches;
			
			// find the end of the ip address
			size_t ip_addr_end = line.find(" ");
			std::string_view ip = line.substr(0, ip_addr_end);

			// move the pointer to after the end of the ip addr
			line.remove_prefix(ip_addr_end + 1);

			
			size_t timestamp_start = line.find("[");
			size_t timestamp_end = line.find("]");

			// skip the open and closing brackets
			std::string_view timestamp = line.substr(timestamp_start+1, timestamp_end- timestamp_start - 1);


			line.remove_prefix(timestamp_end +1);

			size_t quote_start = line.find("\"");
			size_t quote_end = line.find("\"", quote_start + 1);

			std::string_view request = line.substr(quote_start + 1, quote_end - quote_start - 1);

			line.remove_prefix(quote_end + 1);

			// finds first character not matching whitespace
			size_t status_start = line.find_first_not_of(' ');
    		size_t status_end = line.find(' ', status_start);
    		std::string_view status_str = line.substr(status_start, status_end - status_start);

			line.remove_prefix(status_end + 1);

			size_t bytes_start = line.find_first_not_of(' ');
			std::string_view bytes_str = line.substr(bytes_start);

			return log_entry {
				ip,
				timestamp,
				request,
				status_str,
				bytes_str

			};
		}
};

class LogAnalyser{

	public:

		LogAnalyser(): total_bytes(0) {}

		void add_byte(int byte){total_bytes+=byte;}
		
		void add_ip(std::string ip){
			auto curr = ip_freq.find(ip);
			if (curr == ip_freq.end())
				ip_freq[std::string(ip)] = 1;
			else
				ip_freq[std::string(ip)]++;
		}

		void add_status(int code){
			auto curr = status_freq.find(code);
			if (curr == status_freq.end())
				status_freq[code] = 1;
			else
				status_freq[code]++;
		}

		long long total_sent(){
			return total_bytes;
		}

		std::vector<std::pair<int, int>> code_distr(){
			std::vector<std::pair<int, int>> distributions;
			for (auto status: status_freq){
				distributions.push_back({status.first, status.second});
			}
			std::sort(distributions.begin(), distributions.end(), std::greater<std::pair<int,int>>());
			return distributions;
		}

		std::vector<std::pair<std::string_view, int>> top_n(int n){
			int max = (n > ip_freq.size()) ? ip_freq.size() : n;

			std::vector<std::pair<std::string_view, int>> distributions;
			
			for (auto status: ip_freq){
				distributions.push_back({status.first, status.second});
			}

			std::sort(distributions.begin(), distributions.end(),std::greater<std::pair<int,int>>());
			return std::vector<std::pair<std::string_view, int>>(distributions.begin(), distributions.begin()+ max);
		}


	private:
		long long total_bytes;
		std::unordered_map<std::string, int> ip_freq;
		std::unordered_map<int, int> status_freq;




};


