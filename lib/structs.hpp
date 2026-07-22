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

		
	
	private:
		std::map<std::string, std::unique_ptr<std::ifstream>> opened_files;

};

