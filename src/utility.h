#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "defines.h"

int RandomInit(float* data, int size, int range);

int ReadPPM(char* filename, unsigned char frame[IMAGE_HEIGHT * IMAGE_WIDTH * 3], int imgW, int imgH);
//int ReadPPM(char* fileName, unsigned char* frame, int w, int h);

int SavePPM( unsigned char* data, char* fname,int w, int h);
int BatchSavePPM ( unsigned char* imgData, char* filename, int width, int height, int batch);

int printImgRGB(unsigned char* image,int imgWidth, int imgHeight,int batch);
int printFtMapInt(int* ftMap,int ftMapWidth, int ftMapHeight,int ftMapDepth);

int NormFtMap(int* FtMapInt, unsigned char* ftMapChar, int ftMapW, int ftMapH,int channel, int dim, int resolution);

#endif