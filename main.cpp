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
		std::cout<<argc<<std::endl;
		if(!std::filesystem::is_regular_file(argv[i])){
			std::cerr << "Argument:"<< argv[i]<<" is not a file!"<<std::endl;
			return -1;
		}
		files.push_back(argv[i]);
    }
	
	LogParser parser = LogParser(files);
	parser.parse_all();

	return 0;
}