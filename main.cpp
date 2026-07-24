#include <iostream>
#include <vector>
#include "log_parser.cpp"



int main(int argc, char* argv[]){

	if (argc < 2){
		std::cerr << "Insufficient args passed!"<<std::endl;
		return -1;
	}
	std::vector<std::string> files;
	files.reserve(argc);
	for (int i = 1; i < argc; i++) {
		if(!std::filesystem::is_regular_file(argv[i])){
			std::cerr << "Argument:"<< argv[i]<<" is not a file!"<<std::endl;
			return -1;
		}
		files.push_back(argv[i]);
    }
	
	LogParser parser = LogParser(files);
	parser.parse_all();

	std::cout<< "Top 100 IPs:"<<std::endl;
	for (auto ip: parser.top_n(100)){
		std::cout<<"    "<<ip.first<<" : "<<ip.second<<std::endl;
	}

	std::cout<< "Status Code Distribution:"<<std::endl;
	for (auto code: parser.code_distr()){
		std::cout<<"    "<<code.first<<" : "<<code.second<<std::endl;
	}

	std::cout<< "Total Bytes Sent:"<<parser.total_sent()<<std::endl;

	return 0;
}