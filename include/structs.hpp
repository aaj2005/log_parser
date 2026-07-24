#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>
#include <ctime>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <iostream>


struct log_entry {	
		std::string_view ip_address;
		std::string_view timestamp;
		std::string_view request;
		std::string_view status_code;
		std::string_view bytes_sent;
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

		void open_file(std::string full_path){
			auto path = std::filesystem::canonical(full_path).string();
			
			std::unique_ptr<std::ifstream> file = std::make_unique<std::ifstream>(path);
			if (file->is_open()){
				opened_files[path] = std::move(file);
			}
			line_number[path] = 0;
		}

		void close_file(std::string full_path){
			// std::filesystem::canonical ensures that we get the absolute path to a file
			auto path = std::filesystem::canonical(full_path).string();
			auto file = opened_files.find(path); 
			if (file != opened_files.end()){
				file->second->close();
				opened_files.erase(path);
				line_number.erase(path);
			}
		}

		void close_all(){
			auto it = opened_files.begin();

			while(it != opened_files.end()){
				std::string filename = it->first;
				// advance iterator to next file name
				++it;

				// close file 
				close_file(filename);

			}
		}
		
		std::vector<std::string> get_paths(){
			std::vector<std::string> paths;
			for (auto& entry : opened_files){
				paths.push_back(entry.first);
			} 
			return paths;
		}

		std::string getNextLine(std::string file_path){
			auto path = std::filesystem::canonical(file_path).string();
			
			auto& file = opened_files[path];



			std::string line;
			
			// get next line 
			do{
				std::getline(*file, line);
				line_number[path]++;
			}while(line.empty() && !((*file).peek() == EOF)); // skip empty lines
			
			
			// check end of file
			if ((*file).peek() == EOF){
				line_number[path] = 0;
				(*file).clear();
				(*file).seekg(0);
				return "EOF";
			}

			return line;
		}

		int getLineNumber(std::string path){
			return line_number[path];
		}
	

	private:
		std::unordered_map<std::string, std::unique_ptr<std::ifstream>> opened_files;
		std::unordered_map<std::string, int> line_number;
};



class LogAnalyser{

	public:

		LogAnalyser(): total_bytes(0) {}

		void add_byte(std::string_view byte){total_bytes+=std::stoull(std::string(byte));}
		
		void add_ip(std::string_view ip){
			std::string addr = std::string(ip);
			auto curr = ip_freq.find(addr);
			if (curr == ip_freq.end())
				ip_freq[addr] = 1;
			else
				ip_freq[addr]++;
		}

		void add_status(std::string_view status_code){
			int code = std::stoi(std::string(status_code));
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
			std::sort(distributions.begin(), distributions.end(), [](const auto& a, const auto& b){return a.second > b.second;});
			return distributions;
		}

		std::vector<std::pair<std::string, int>> top_n(int n){
			int max = (n > ip_freq.size()) ? ip_freq.size() : n;

			std::vector<std::pair<std::string, int>> distributions;
			
			for (auto status: ip_freq){
				distributions.push_back({status.first, status.second});
			}

			std::sort(distributions.begin(), distributions.end(),[](const auto& a, const auto& b){return a.second > b.second;});
			return std::vector<std::pair<std::string, int>>(distributions.begin(), distributions.begin()+ max);
		}


	private:
		long long total_bytes;
		std::unordered_map<std::string, int> ip_freq;
		std::unordered_map<int, int> status_freq;




};



#endif