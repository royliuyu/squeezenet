#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"
#include "defines.h"


//**********************************************************************
// Function Name: decode_image                                         *
// Description: - read image in ppm formate, read the data of array    *
//                named frame[]                                        *
// Input file: image file : viptrafficX.ppm                            *
// Output file: none                                                   *
// Return: 0 if success                                                *
//**********************************************************************

int readPPM(unsigned char frame[IMAGE_HEIGHT * IMAGE_WIDTH * 3], char filename[])
{
	FILE *pFile;
	pFile = fopen(filename, "r");
	fseek(pFile, 15L, SEEK_SET);//In ppm file, the first 15 bytes are content of "p6,120 160, 255", image data is from 16th bytes

	fread(frame, sizeof(unsigned char), IMAGE_HEIGHT * IMAGE_WIDTH * 3+15, pFile); 
	fclose(pFile);
	return 0;
}


//**********************************************************************
// Function Name:randomInit                                            *
// Description: - Generate random value to an float array              *
//                                                                     *
// Input file: none                                                    *
// Output file: none                                                   *
// Return: kernel file size                                            *
//**********************************************************************
int randomInit(float* data, int size, int range) // random form 0/255 to 255/255
{
	int i;
	srand(time(NULL));
	for (i = 0; i < size; i++) 
	{
		data[i] = rand() % range / (float)range;
	}
	//for (i = 0; i < size; i++) printf("%f;", data[i]); // for debugging
	return 0;
}
