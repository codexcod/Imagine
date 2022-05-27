#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

#define BLACK 0

using namespace std;

// Filtro plano como ejemplo

void plain(ppm& img, unsigned char c)
{

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));

}

// Dada una imagen .ppm, la convierte en blanco y negro
void blackWhite(ppm& img)
{

	// Variable auxiliar para determinar el valor del pixel a convertir
	int auxPixel;

	for (int y = 0; y < img.height; y++) {

		for(int x = 0; x < img.width; x++) {

			// Obtengo el promedio de valor RGB
			// sumando los valores y dividiendolos por 3
			auxPixel = img.getPixel(y,x).cumsum() / 3;
			img.setPixel(y, x, pixel(auxPixel, auxPixel, auxPixel));
		}

	}
}
