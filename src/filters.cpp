#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

using namespace std;

// Filtro plano como ejemplo
void plain(ppm& img, unsigned char c)
{
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));

}

// Dada una imagen .ppm, un comienzo y un final
// convierte esa parte en blanco y negro
void blackWhite(ppm& img, int start, int end)
{

	// Variable auxiliar para determinar el valor del pixel a convertir
	int auxPixel;

	for (int y = start; y < end; y++) {

		for(int x = 0; x < img.width; x++) {

			// Obtengo el promedio de valor RGB
			// sumando los valores y dividiendolos por 3
			auxPixel = img.getPixel(y,x).cumsum() / 3;
			img.setPixel(y, x, pixel(auxPixel, auxPixel, auxPixel));
		}

	}
}

void blackWhiteMultiThread(ppm& img, int n_threads)
{

	int rows_for_thread = (int)(img.height / n_threads);
	vector<thread> threads_result;

	for (int i = 0; i < n_threads; i++)
	{
		// Calculo el principio y el final
		int start = i * rows_for_thread;
		int end = (i + 1) * rows_for_thread;

		threads_result.push_back(thread(blackWhite, ref(img), start, end));
	}

	for (int i = 0; i < n_threads; i++)
	{
        threads_result[i].join();
	}
}

void merge(ppm& img1, ppm& img2, float alpha)
{

	float p1 = alpha;
	float p2 = 1 - p1;

	for(int i = 0; i < img1.height; i++)
	{
		for(int j = 0; j < img1.width; j++)
		{	
			int r1 = img1.getPixel(i, j).r;
			int g1 = img1.getPixel(i, j).g;
			int b1 = img1.getPixel(i, j).b;
			
			int r2 = img2.getPixel(i, j).r;
			int g2 = img2.getPixel(i, j).g;
			int b2 = img2.getPixel(i, j).g;

			int r3 = r1 * p1 + r2 * p2;
			int g3 = g1 * p1 + g2 * p2;
			int b3 = b1 * p1 + b2 * p2;
			
			img1.setPixel(i,j, pixel(r3, g3, b3).truncate());
		}		
	}
}