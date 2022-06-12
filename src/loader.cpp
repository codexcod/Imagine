#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <filesystem>
#include "filters.h"
#define ONE_OVER_BILLION 1E-9


using namespace std;
using namespace filesystem;

int main(int argc , char* argv[]){
	
		cout << argc << endl;
	
	if(string(argv[1]) == "--help" || string(argv[1]) == "-h"){
		cout << "Usage: ./loader <filtro> <n_threads> <p1> <dir> <out_dir> <p2>" << endl;
		return 0; 
	}
	
	string filter = string(argv[1]);
	unsigned int n_threads = atoi(argv[2]);
	float p1 = atof(argv[3]);
	string dir = string(argv[4]);
	path out_dir = path(dir);
	if(argv[5]) out_dir = path(string(argv[5]));
	float p2;

	if (argc == 7)
	{
		p2 = atof(argv[6]);
	}

	// Create the directory if it doesn't exist.
	if (!is_directory(out_dir)) create_directory(out_dir);

	
    for (const auto & image : directory_iterator(dir)){
		auto img_path = image.path();
		if (img_path.extension() != ".ppm") continue;

        ppm img(img_path);
		
		if (filter == "plain") 
		{
			plain(img, (unsigned char)p1);
		}
		else if (filter == "BYN") 
		{
			blackWhiteMultiThread(img, n_threads);
		} 
		else if (filter == "shades")
		{
			shades(img, p1);
		}
		else if (filter == "zoom")
		{
			ppm img_aux(img);
			zoom(img, img_aux, p1);
		}
		else if (filter == "crop")
		{
			crop(img, p1, p2);
		}
		else if (filter == "ed")
		{
			edgeDetectionMultiThread(img, n_threads);
		}
	
		path out_path = out_dir; 
		out_path /= string(img_path.stem()) + "_" + filter + ".ppm";
		img.write(out_path);	
	}	
	cout << "DONE!" << endl;
	return 0;
}


