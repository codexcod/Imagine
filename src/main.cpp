#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#define ONE_OVER_BILLION 1E-9


using namespace std;

// El siguiente es un template basico que pueden usar como base

int main(int argc , char* argv[]){
	
	// Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
	// Asumimos que Zoom no se puede encadenar

	if(string(argv[1]) == "-help"){
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
		return 0; 
	}
	
	string filter = string(argv[1]);
	unsigned int n_threads = atoi(argv[2]);
	bool single_thread = n_threads == 1;
	float p1 = atof(argv[3]);
	string img1_str(argv[4]);
	ppm img1(img1_str);
	string out = string(argv[5]);
	float p2;
	string img2_str;
	ppm img2;

	if (argc == 8)
	{
		p2 = atof(argv[6]);
		img2_str = argv[7];
		img2 = ppm(img2_str);
	}
	

	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

	if (filter == "plain") 
	{
		plain(img1, (unsigned char)p1);
	}
	else if (filter == "BYN") 
	{
		// Blanco y Negro
		// ./main BYW <n_threads> 0 <ruta_img> <ruta_img_result>

		// El mejor resultado fue con 3 threads en 0.013076s, mas threads empeora
		// Testeado en procesador intel i3-4170 de 2 nucleos con 4 threads.

		// Singlethread fue de 0.022190s.

		if (single_thread)
			blackWhite(img1, 0, img1.height);
		else
			blackWhiteMultiThread(img1, n_threads);
	} 
	else if (filter == "merge") 
	{
		// Merge de imagenes
		// ./main merge 1 <alpha> <ruta_img1> <ruta_img_result> 0 <ruta_img2>

		merge(img1, img2, p1);
	} 
	else if (filter == "zoom")
	{
		// Digital zoom de una imagen
		// ./main zoom 1 <cantidad_zoom> <ruta_img1> <ruta_img_result>

		ppm img(img1);
		zoom(img1, img, p1);
	}

   	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "Escribiendo imagen" << endl;
	img1.write(out);
	    
	cout << "Listo" << endl;
	return 0;
}