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
#include "main.h"
#include "convol.h"
#include "defines.h"
#include "utility.h"
#include "relu.h"
#include "padding.h"
#include "MAXpool.h"
#include "concat.h"


//**********************************************************************
// Function Name: mian.c                                               *
// Description: - Main function on host, configure the kernel parameter*
//                and run kernel                                       *
// Input file: none                                                    *
// Output file: none                                                   *
// Return: 0 if success                                                *
//**********************************************************************
int main(int argc, char** argv) {
	
	imageCount = 0;

//	read 160*120 ppm pictures , total 120 frames
//	while (imageCount<10) {
//	char filename[50];//filename length upto 50
//	sprintf(filename, "images/viptraffic%d.ppm", imageCount);//read viptrafficX.ppm
//	image = (unsigned char*)malloc(imageHeight * imageWidth * sizeof(unsigned char) * 3);
//	readPPM(image, filename); //get image data from file
	
	//read image
	imgWidth = IMAGE_WIDTH ; //image is color's with RGB
	imgHeight = IMAGE_HEIGHT;
	image = (unsigned char*)malloc(imgHeight * imgWidth * 3 * sizeof(unsigned char));
	ReadPPM(image, "uncc.ppm");

	//image processing, add padding =3 for processing, RGB. 
	//In 224*224*3, Out 227*227*3, filter: none
	imgLayer = (unsigned char*)malloc((imgHeight+3)* (imgWidth+3) * 3 * sizeof(unsigned char));
	PadImageRGB(image,imgLayer, imgWidth, imgHeight, 3); //transfer size from 224x224 to 227x227 pad a image , add totally 3 pad, RGB 
	imgLayerWidth = imgWidth+3;
	imgLayerHeight = imgHeight+3;
	free(image);
	image = NULL;
	
	//convolution 1, 96 kernals, kernel size 7, padding 0, stride 2
	//In 227*227*3, filter 7*7*96 -> Out 111*111*96£¬
	conv1In = imgLayer;
	conv1Width = (imgLayerWidth - 7 +0)/2+1; //kernel 7*7, padding 0, stride 2
	conv1Height = (imgLayerHeight - 7 +0)/2+1; 
	conv1Out =	(int *)malloc(96 * conv1Width * conv1Height *sizeof(int));
	fltConv1 = (float*)malloc(3  *  7 * 7 * 96 * sizeof(float));
	RandomInit(fltConv1, (7 * 7 * 96), 255);
	ConvRGB(conv1In,conv1Out,fltConv1,imgLayerWidth,imgLayerHeight, 96, 7, 2);  //imgLayerWidth is with padding
	free(imgLayer);
	conv1In = NULL;
	imgLayer = NULL;


	
	//max pool, pool1 : pool one, filter 3, padding 0, stride 2
	//In 111*111*96, Out: 55*55*96, filter: none
	p1In = conv1Out;
	p1Width = (conv1Width - 3 + 0)/2 +1; //55
	p1Width = (conv1Width - 3 + 0)/2 +1;
	p1Out = (int*)malloc(96 * p1Width * p1Height * sizeof(int)); //
	MAXpool(p1In, p1Out, 3, 2, 0);
	free (conv1Out);
	p1In = NULL;
	conv1Out = NULL;


	//fire2 squeeze , 16 dims, 128 kernels/dim, kernel size 1, padding 0, stride 1
	//In 55*55*96 , filter 1*1*96*16 -> out 55*55*16
	f2SqWidth = p1Width; //1x1 convolution
	f2SqHeight = p1Height;
	f2SqIn= p1Out;
	f2SqOut = (int*)malloc(16 * f2SqWidth * f2SqHeight * sizeof(int));
	fltF2Sq = (float*)malloc(1 * 1 * 96 * 16 * sizeof(float));
	RandomInit(fltF2Sq, (1 * 1 * 96 * 16 ), 255);
	Conv1x1(f2SqIn, f2SqOut,fltF2Sq, f2SqWidth, f2SqHeight,16,1,96);
	free (p1Out);
	f2SqIn = NULL;
	p1Out = NULL;

	//fire2 expand 1 x 1:  64 dims, 16 kernels/dim, kernel size 1, padding 0, stride 1
	//in 55*55*64, filter 1*1*64*16 -> out 55*55*64
	f2Ex1Width = f2SqWidth;
	f2Ex1Height = f2SqHeight;
	f2Ex1In = f2SqOut; // connect f2 squeeze output
	f2Ex1Out = (int*)malloc(64* f2Ex1Width * f2Ex1Height * sizeof(int));
	fltF2Ex1 = (float*)malloc(64 * 16 *  1 * 1 * sizeof(float));
	RandomInit(fltF2Ex1, (64 * 16 * 1 * 1), 255);
	Conv(f2Ex1In, f2Ex1Out,fltF2Ex1, f2Ex1Width, f2Ex1Height,64,16,1,0,1); 
	f2Ex1In = NULL;

	//fire2 expand 3 x 3:  64 dims, 16 kernels/dim, kernel size 3, padding 0, stride 1
	//in 55*55*64, filter 3*3*64*16 -> out 55*55*64
	f2Ex3Width = f2SqWidth; 
	f2Ex3Height = f2SqHeight;
	f2Ex3In = f2SqOut;
	f2Ex3Out = (int*)malloc(64 * f2Ex3Width * f2Ex3Height * sizeof(int));
	fltF2Ex3 = (float*)malloc(64 * 16 * 3  * 3 *  sizeof(float));
	RandomInit(fltF2Ex1, (64 * 16 * 3 * 3), 255);
	Conv(f2Ex3In, f2Ex3Out,fltF2Ex3, f2Ex3Width, f2Ex3Height,64, 16, 3, 0,1); 
	free(f2SqOut);
	f2Ex3In = NULL; 
	f2SqOut = NULL;

	//Concat fire2 expand 1 and expand 3
	f2Out = (int*) malloc(f2Ex1Width*f2Ex1Height*128*sizeof(int));
	if (f2Ex1Width != f2Ex3Width | f2Ex1Height != f2Ex3Height) 
	{
		printf("concat failed\n");
		return 1;
	}
	else Concat (f2Out, f2Ex1Out, f2Ex3Out,55,55,64,64); 

	/* fire 3 .....*/

	imageCount ++;

//	} // end of while, which process 120 images 
	
	// Output..........

	printf ("Done!");

	//clear memory

	free(fltConv1);
	free(fltF2Sq);
	free (fltF2Ex1);
	free (fltF2Ex3);

	return 0;
}
