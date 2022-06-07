#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void blackWhite(ppm& img, int start, int end);
void shades(ppm& img, unsigned char shades);
void merge(ppm& img1, ppm& img2, float alpha);
void edgeDetection(ppm &img, ppm &img_target);
void plain(ppm &img, unsigned char c);
void zoom(ppm &img, ppm &img_zoomed, int n);
void crop(ppm &img, int rows, int columns);

// MULTI-THREAD FILTERS

void blackWhiteMultiThread(ppm& img, int n_threads);
void edgeDetectionMultiThread(ppm &img, ppm &img_target, int n_threads);

#endif