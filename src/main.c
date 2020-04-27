//**********************************************************************
//                                                                     *
//               University Of North Carolina Charlotte                *
//                                                                     *
//Program: main for squeezenet                                         *
//Description: This program is to do squeezenet                        *
//                                                                     *
//File Name: mian.cpp                                                  *
//File Version: 1.0                                                    *
//Baseline:                                                            *
//                                                                     *
//                                                                     *
//Input file: images/viptraffic0.ppm ...  images/viptraffic119.ppm     *
//Output file: none                                                    *
//**********************************************************************  


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "convol.h"
#include "defines.h"
#include "utility.h"
#include "relu.h"


//**********************************************************************
// Function Name: mian.c                                               *
// Description: - Main function on host, configure the kernel parameter*
//                and run kernel                                       *
// Input file: none                                                    *
// Output file: none                                                   *
// Return: 0 if success                                                *
//**********************************************************************
int main(int argc, char** argv) {

	int imageWidth, imageHeight, kernelSize,convWidth, convHeight, padding, stride;
	int imageCount;
	float* kernel;
	float* convOutput;
	float* reluOutput;
	unsigned char* image;

	int kernalSize_fire2Sq; //kerel size of fire2 squeeze, 1 * 1
	float* filterFire2Sq; // 16 kernals
	float* fire2SqIn;
	float* fire2SqOut;
	int convWidth_fire2Sq;
	int convHeight_fire2Sq;

	int kernalSize_fire2Expd1; //kerel size of fire2 squeeze, 1 * 1
	float* filterFire2Expd1; // 16 kernals
	float* fire2Expd1In;
	float* fire2Expd1Out;
	int convWidth_fire2Expd1;
	int convHeight_fire2Expd1;

	int kernalSize_fire2Expd3; //kerel size of fire2 squeeze, 3 * 3
	float* filterFire2Expd3; // 16 kernals
	float* fire2Expd3In;
	float* fire2Expd3Out;
	int convWidth_fire2Expd3;
	int convHeight_fire2Expd3;


	imageCount = 0;
	imageWidth = IMAGE_WIDTH;
	imageHeight = IMAGE_HEIGHT;

	while (imageCount<10) {
	char filename[50];//filename length upto 50
	sprintf(filename, "images/viptraffic%d.ppm", imageCount);//read viptrafficX.ppm
	image = (unsigned char*)malloc(imageHeight * imageWidth * sizeof(unsigned char) * 3);
	readPPM(image, filename); //get image data from file



	imageWidth = IMAGE_WIDTH * 3; //image is color's with RGB
	imageHeight = IMAGE_HEIGHT * 3;
	padding = 0;
	stride =1;
	kernelSize =3;
	convWidth = (imageWidth - kernelSize + 2 * padding) / stride + 1;
	convHeight = (imageHeight - kernelSize + 2 * padding) / stride + 1;  


	/*convolution 1
	...
	*/

	/* max pools 1
		...
	*/

	//fire2 squeeze , filter:  3*  1*1*16

	convWidth_fire2Sq= imageWidth * 16;
	convHeight_fire2Sq= imageWidth * 16;
	kernalSize_fire2Sq = 1 *1;
	filterFire2Sq = (float*)malloc(3  *  1 * 1 * 16 * sizeof(float));
	fire2SqIn = (float *) image; // connect to previous layer's output
	fire2SqOut = (float*)malloc(convWidth_fire2Sq * convHeight_fire2Sq * sizeof(float));
	randomInit(filterFire2Sq, (kernalSize_fire2Sq*16), 255); //initialize kernel, 16 kernels
	convolute(fire2SqIn,  filterFire2Sq, fire2SqOut, convWidth_fire2Sq, convHeight_fire2Sq,16,1,0,1); // kernal number 16, kernel size 1, padding 0, stride 1
	relu(fire2SqOut,(convWidth_fire2Sq * convHeight_fire2Sq)); //relu data size = size of fire2_sqOut

	//fire2 expand 1 x 1, filter: 3*  1*1*64


	convWidth_fire2Expd1= convWidth_fire2Sq * 64; // 64 channels for 64 number of kernel
	convHeight_fire2Expd1= convHeight_fire2Sq * 64;
	kernalSize_fire2Expd1 = 1*1;
	filterFire2Expd1 = (float*)malloc(3  *  1 * 1 * 64 * sizeof(float));
	fire2Expd1In = fire2SqOut; // connect to the output of fire2 squeeze
	fire2Expd1Out = (float*)malloc(convWidth_fire2Expd1 * convHeight_fire2Expd1 * sizeof(float)); //filter is 64 of kernel
	randomInit(filterFire2Expd1, (kernalSize_fire2Expd1*64), 255); //initialize kernel, 16 kernels
	convolute(fire2Expd1In,  filterFire2Expd1, fire2Expd1Out, convWidth_fire2Expd1, convHeight_fire2Expd1,64,1,0,1); // kernal number 64, kernel size 1, padding 0, stride 1
	relu(fire2Expd1Out,(convWidth_fire2Expd1 * convHeight_fire2Expd1 )); 

	//fire2 expand 3 x 3, filter: 3*  1*1*64


	convWidth_fire2Expd3= convWidth_fire2Expd1 * 64; // 64 channels for 64 number of kernel
	convHeight_fire2Expd3= convHeight_fire2Expd1 * 64;
	kernalSize_fire2Expd3 = 3*3;
	filterFire2Expd3 = (float*)malloc(3  *  3 * 3 * 64 * sizeof(float));
	fire2Expd3In = fire2Expd1Out; // connect to the output of fire2 expand 1X1 layer
	fire2Expd1Out = (float*)malloc(convWidth_fire2Expd3 * convHeight_fire2Expd3 * sizeof(float)); //filter is 64 of kernel
	randomInit(filterFire2Expd3, (kernalSize_fire2Expd3*64), 255); //initialize kernel, 64 kernels
	convolute(fire2Expd3In,  filterFire2Expd3, fire2Expd3Out, convWidth_fire2Expd3, convHeight_fire2Expd3,64,1,0,1); // kernal number 64, kernel size 1, padding 0, stride 1
	relu(fire2Expd3Out,(convWidth_fire2Expd3 * convHeight_fire2Expd3 )); 


	imageCount ++;

	} // end of 120 images processing
	
	// Output..........

	printf ("Done!");

	//clear memory


	free (filterFire2Sq);
	free (fire2SqOut);
	free (filterFire2Expd1);
	free (fire2Expd1Out);
	free (filterFire2Expd3);
	free (fire2Expd3Out);

	return 0;
}
