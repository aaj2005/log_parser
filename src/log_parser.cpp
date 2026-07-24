
#include "structs.hpp"

class LogParser{

	public:

		LogParser(std::vector<std::string> files): reader(LogReader()), analyser(LogAnalyser())  {
			
			for (std::string file : files){
				reader.open_file(file);
			}

		}
		~LogParser(){reader.close_all();}

		void parse_all(){
			std::vector<std::string> files = reader.get_paths();
			for (std::string file: files){
				std::string line = "";
				while (line != "EOF"){
					line = reader.getNextLine(file);
					log_entry entry = parseLine(line);

					analyser.add_ip(entry.ip_address);
					analyser.add_status(entry.status_code);
					analyser.add_byte(entry.bytes_sent);


					
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

		LogReader reader;
		LogAnalyser analyser;

};

