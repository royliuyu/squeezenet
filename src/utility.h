#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "defines.h"

int RandomInit(float* data, int size, int range);
int ReadPPM(unsigned char frame[IMAGE_HEIGHT * IMAGE_WIDTH * 3], char filename[]);

#endif