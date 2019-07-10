#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

inline bool is_jpg(struct dirent *dirp) {
	int len = strlen(dirp->d_name);
	return (dirp->d_name[len-4] =='.'
	&& dirp->d_name[len-3] =='j'
	&& dirp->d_name[len-2] =='p'
	&& dirp->d_name[len-1] =='g');
}

inline bool is_png(struct dirent *dirp) {
	int len = strlen(dirp->d_name);
	return (dirp->d_name[len-4] =='.'
	&& dirp->d_name[len-3] =='p'
	&& dirp->d_name[len-2] =='n'
	&& dirp->d_name[len-1] =='g');
}

inline bool is_custome_filetype(struct dirent*dirp, const char* type) {
	int type_len = strlen(type);
	int len = strlen(dirp->d_name);
	for(int i = 0; i < type_len; ++i) {
		if(dirp->d_name[len - type_len + i] != type[i]) return false;
	}
	return true;
}

inline bool exists_file(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "%s [input_dir] [out_dir]\n", argv[0]);
		return -1;
	}

	DIR *dp;
    struct dirent *dirp;
    std::string dirname(argv[1]);
    if((dp = opendir(dirname.c_str())) == NULL) {
        std::cout << "Can't open " << dirname << std::endl;
    }
    int count = 0;
    std::ofstream ferr;
    ferr.open("license_run.log");

    std::cout << "-- start:" << dirname << "--"<<std::endl;
    while((dirp = readdir(dp)) != NULL) {
		if(argc >= 4) {
			if(!is_custome_filetype(dirp, argv[3])) continue;
		} else {
    		if(!is_png(dirp) && !is_jpg(dirp)) continue;
		}
        std::string origin_img(dirname+"/"+dirp->d_name);
		std::string rawFilename = dirp->d_name;
		std::string out_prefix(argv[2]);
        std::cout << rawFilename << std::endl;
		ferr << origin_img << "->" << out_prefix+"/"+rawFilename << std::endl;
        count++;
    }
	closedir(dp);
    ferr.close();
    std::cout << "-- license autogen json "<< dirname <<" total file number is " << count << std::endl;
	return 0;
}