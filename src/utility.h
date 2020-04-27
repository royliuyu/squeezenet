#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "defines.h"

int randomInit(float* data, int size, int range);
int readPPM(unsigned char frame[IMAGE_HEIGHT * IMAGE_WIDTH * 3], char filename[]);

#endif