#include <string>
#include <iostream>
#include <utility>
#include <fstream>
std::pair<std::string, int> split_line(std::string& line) {
	int len = line.size();
	if(len <= 0) std::cerr << "Error" << std::endl;
	int idx = 0;
	while(line[idx]!=' ') idx++;
	std::string img_path = line.substr(0, idx); 
	while(line[idx]==' ') idx++;
	int label = int(line.substr(idx, idx+1)[0] - '0');
	return {img_path, label};
}
std::string abs_path(std::string& path, std::string prefix="/home/xxxx/") {
	return  prefix + path;
}
int main(int argc, char const *argv[]) {
	
	if(argc <= 1) {
		std::cerr << "please input a file\n";
		return -1;
	}
    std::ifstream infile;
    infile.open(argv[1], std::ios::in);
    if(!infile.is_open()) {
    	std::cerr << "open file failed" << std::endl;
    	return -1;
 	}
 	int c = 0;
 	std::string tmp;
 	std::pair<std::string, int> img_label;
    while(!infile.eof()){
        getline(infile, tmp, '\n');
        c++;
        if(tmp.length() == 0) continue;
        img_label = split_line(tmp);
        int label = img_label.second;
        std::cout << abs_path(img_label.first) << std::endl;
        std::cout << label << std::endl;
    }

	infile.close();
	return 0;
}