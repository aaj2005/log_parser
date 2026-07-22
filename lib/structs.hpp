#include <string>
#include <ctime>
#include <map>
#include <fstream>
#include <filesystem>


struct log_entry {	
		std::string ip_address;
		time_t timestamp;
		std::string http_method;
		std::string url;
		unsigned short status_code;
		int bytes_sent;
};

class LogReader{

/*
	LogReader class:
	
	Class manages lifecycle of all opened log files.
	Upon destruction, all opened files are closed and deleted

*/ 
	public:

		~LogReader() {
			close_all();
		} 

		void open_file(std::string path){
			auto path = std::filesystem::canonical(path);
			
			std::unique_ptr<std::ifstream> file = std::make_unique<std::ifstream>(file);
			if (file->is_open()){
				opened_files[path] = std::move(file);
			}
		}

		void close_file(std::string path){
			// std::filesystem::canonical ensures that we get the absolute path to a file
			auto path = std::filesystem::canonical(path);
			auto file = opened_files.find(path); 
			if (file != opened_files.end()){
				file->second->close();
				opened_files.erase(path);
			}

		}

		void close_all(){
			for (const auto& entry : opened_files){
				close_file(entry.first);
			}
		}

		std::string getNextLine(std::string path){
			auto path = std::filesystem::canonical(path);
			
			auto& file = opened_files[path];


			// check end of file
			if ((*file).peek() == EOF){
				(*file).clear();
				(*file).seekg(0);
				return "EOF";
			}

			std::string line;
			
			// get next line 
			std::getline(*file, line);

			if (line_number.find(path) == line_number.end()){
				line_number[path] = 1;
			}else{
				line_number[path]++;
			}

			return line;
		}

		int getLineNumber(std::string path){
			return line_number[path];
		}


	private:
		std::map<std::string, std::unique_ptr<std::ifstream>> opened_files;
		std::map<std::string, int> line_number;
};



