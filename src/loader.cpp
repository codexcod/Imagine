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

	if(string(argv[1]) == "--help" || string(argv[1]) == "-h"){
		cout << "Usage: ./loader <filter> <p1> <n_threads> <root_dir> <output_dir>" << endl;
		return 0; 
	}
	
	string filter = string(argv[1]);
	float p1 = atof(argv[2]);
	unsigned int n = atoi(argv[3]);
	string dir = string(argv[4]);
	path out_dir = path(dir);
	if(argv[5]) out_dir = path(string(argv[5]));

	// Create the directory if it doesn't exist.
	if (!is_directory(out_dir)) create_directory(out_dir);

	
    for (const auto & image : directory_iterator(dir)){
		auto img_path = image.path();
		if (img_path.extension() != ".ppm") continue;

        ppm img(img_path);
		if (filter == "plain") plain(img, (unsigned char)p1);
		else if (filter == "blackwhite") blackWhite(img);
		else if (filter == "shades") shades(img, (unsigned char)p1);
		else if (filter == "contrast") contrast(img, (float)p1);
		else if (filter == "bright") brightness(img, (float)p1);
		else if (filter == "frame") frame(img, (int)p1);
		else if (filter == "zoom") zoom(img, (int)p1);
		else if (filter == "blur") boxBlur(img, p1);	
		else if (filter == "edge") edgeDetection(img);	
	
		path out_path = out_dir; 
		out_path /= string(img_path.stem()) + "_" + filter + ".ppm";
		img.write(out_path);	
	}	
	cout << "DONE!" << endl;
	return 0;
}

void convolution(ppm& img, int kernel[3][3]) {
	ppm out_img (img.width-2, img.height-2);
		
	for(int y = 1; y < img.height-1; y++) {
		for(int x = 1; x < img.width-1; x++) {
			pixel summation;
			for(int i = -1; i <=1; i++) {
				for(int j = -1; j <=1; j++) {
					summation.addp(img.getPixel(i+y, j+x).mult(kernel[i+1][j+1])); 
				}
			}
			out_img.setPixel(y-1,x-1, summation.mult((float)1/9).truncate());
		}
	}
	img = out_img;
}


void boxBlur(ppm &img, int iterations=1) {
	int kernel[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1}
	};
	for (int i=0; i<=iterations; i++) convolution(img, kernel);
}


void edgeDetection(ppm &img) {
	shades(img, 8);
	boxBlur(img, 5);
	int kernel_x[3][3] = {
		{1,0,-1},
		{2,0,-2},
		{1,0,-1}
	};
	int kernel_y[3][3] = {
		{1, 2, 1},
		{0, 0, 0},
		{-1, -2, -1}
	};
	ppm img_x = img;
	ppm img_y = img;
	
	convolution(img_x, kernel_x);	
	convolution(img_y, kernel_y);

	for(int y = 1; y < img.height-4; y++) {
		for(int x = 1; x < img.width-4; x++) {
			pixel pixel_x = img_x.getPixel(y,x);
			pixel pixel_y = img_y.getPixel(y,x);
			
			pixel_x = pixel_x.power(2);
			pixel_y = pixel_y.power(2);
			pixel result = pixel_x;
			
			result = result.addp(pixel_y).power(((float)1/2));
			img.setPixel(y, x, result);
		}
	}
}
