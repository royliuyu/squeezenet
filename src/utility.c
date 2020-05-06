#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "utility.h"
#include "defines.h"



//**********************************************************************
// Function Name: ReadPPM                                              *
// Description: - read image in ppm formate, read the data of array    *
//                named frame[]                                        *
// Input file: image file : XXX.ppm                                    *
// Output file: none                                                   *
// Return: 0, if success                                               *
//**********************************************************************


int ReadPPM(char* filename, unsigned char frame[IMAGE_HEIGHT * IMAGE_WIDTH * 3], int imgW, int imgH)
{
	FILE* pFile;
	pFile = fopen(filename, "rb");
	fseek(pFile,15L, SEEK_SET);//In uncc.ppm file, the first 15 bytes are content of "p6,224 224, 255", image data is from 16th bytes
	//fread(frame, sizeof(unsigned char),IMAGE_HEIGHT * IMAGE_WIDTH * 3, pFile); 
	fread(frame,1 ,imgW * imgH* 3,pFile); 
	fclose(pFile);
	return 0;
}
/*
int ReadPPM(char* fileName, unsigned char* frame, int w, int h)
{
    char header[1024];
    FILE* pFile = NULL;
    int line = 0;
    pFile = fopen(fileName, "rb");
    while(line < 2)
	{   
		fgets(header, 1024, pFile);
        if(header[0] != '#')
		{
            ++line;
        }
    }
    sscanf(header,"%d %d\n", w, h);
    fgets(header, 20, pFile);
    // get rgb data
    fread(frame,w*h*3, 1, pFile);
    fclose(pFile);
	return 0;
}
*/

//**********************************************************************
// Function Name: SavePPM                                              *
// Description: - save image in ppm formate, read the data of array    *
//                named frame[]                                        *
// Input file: none                                                    *
// Output file: XXX.ppm                                                *
// Return: 0, if success                                               *
//**********************************************************************
int SavePPM( unsigned char* data, char* fname,int w, int h)
{
    FILE* pFile;

    pFile = fopen(fname, "wb");
    fprintf(pFile,"P6\n%d %d\n255\n",w,h);
    fwrite(data, w*h*3, 1, pFile);
    fclose(pFile);
	return 0;
}

//**********************************************************************
// Function Name: BatchSavePPM                                         *
// Description: - save images in ppm formate, read the data of array   *
//                named frame[]                                        *
// Input file: none                                                    *
// Output file: XXX_1.ppm, XXX_2.ppm, XXX_3.ppm  ...                   *
// Return: 0, if success                                               *
//**********************************************************************
int BatchSavePPM (unsigned char* imgData, char* filename, int width, int height, int batch) //grey picture
{
	char filenameB[40]; //filename upto 40 characters
	unsigned char* dataB;
	int i;

	dataB= (unsigned char*) malloc((width * height *3+15)*sizeof(unsigned char));

	for (i=0;i<5; i++)
	{
		FILE* pFile;
		//dataB= data + i*w*h*3*sizeof(unsigned char); //move to next batch of image
		dataB= &imgData[i*width * height * 3]; //move to next batch of image
		sprintf(filenameB, "%s_%d.pgm", filename, i);
		pFile = fopen(filenameB, "wb");
		fprintf(pFile,"P5\n%d %d\n255\n",width,height);
		fwrite(imgData, width *height * 3, 1, pFile);
		fclose(pFile);
	}
	return 0;
}


//**********************************************************************
// Function Name:randomInit                                            *
// Description: - Generate random value to an float array              *
//                                                                     *
// Input file: none                                                    *
// Output file: none                                                   *
// Return: 0, if success                                               *
//**********************************************************************
int RandomInit(float* data, int size, int range) // random form 0/255 to 255/255, resolution is 1/255
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

//**********************************************************************
// Function Name:printImgRGB                                           *
// Description: - print char array by batches                          *
//                                                                     *
// Input file: image, char array                                       *
// Output file: none                                                   *
// Return: 0, if success                                               *
//**********************************************************************
int printImgRGB(unsigned char* image,int imgWidth, int imgHeight,int batch)
{
	int i,j;
	for (i=0;i<batch;i++)
	{
		printf("batch%d: \n",i);
		for (j=0;j<imgWidth*imgHeight*3;j++)
		{
			printf ("#%d: %d,  ",j,image[i*imgWidth*imgHeight*3+j]);
		}
		printf("\n");
	}
	return 0;
}


//**********************************************************************
// Function Name: printFtMapInt                                        *
// Description: - print feature map in int array formate               *
//                                                                     *
// Input file: image, int array                                        *
// Output file: none                                                   *
// Return: 0, if success                                               *
//**********************************************************************
int printFtMapInt(int* ftMap,int ftMapWidth, int ftMapHeight,int ftMapDepth)
{
	int i,j;
	for (i=0;i<ftMapDepth;i++)
	{
		printf("batch%d: \n",i);
		for (j=0;j<ftMapWidth*ftMapHeight;j++)
		{
			printf ("#%d: %d,  ",j,ftMap[i*ftMapWidth*ftMapHeight + j]);
		}
		printf("\n\n\n\n");
	}
	return 0;
	system("pause");
}

//**********************************************************************
// Function Name: NormFtMap                                            *
// Description: normalization for visualization, output value with 255 *
//                                                                     *
// Input :  featureMap array                                           *
// Output : ftMapNorm arry                                             *
// Return: 0, if success                                               *
//**********************************************************************

int NormFtMap(int* FtMapInt, unsigned char* ftMapChar, int ftMapW, int ftMapH,int channel, int dim, int resolution)
{
	int i, value;
	assert(channel ==1);
	for (i=0;i<ftMapW*ftMapH*dim;i++)
	{
		value = (float)FtMapInt[i]/(float)resolution*255;
		ftMapChar[i]= value;
	}
	return 0;
}

