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
	readPPM(image, "uncc.ppm");

	//image processing, add padding =3 for processing, RGB
	imgLayer = (unsigned char*)malloc((imgHeight+3)* (imgWidth+3) * 3 * sizeof(unsigned char));
	PadImage(image,imgLayer, imgWidth, imgHeight, 3); //transfer size from 224x224 to 227x227 pad a image , add totally 3 pad, RGB 
	imgLayerWidth = imgWidth+3;
	imgLayerHeight = imgHeight+3;

	//convolution 1, 96 kernals, kernel size 7, padding 0, stride 2
	conv1In= imgLayer;
	conv1Width = (imgLayerWidth - 7 +0)/2+1; //kernel 7*7, padding 0, stride 2
	conv1Height = (imgLayerHeight - 7 +0)/2+1; 
	conv1Out =	(unsigned char*)malloc(3 * conv1Width * conv1Height *sizeof(unsigned char));
	conv(conv1In,conv1Out,fltConv1,imgLayerWidth,imgLayerHeight, 96, 7, 0, 2); 
	
	//max pool, pool1 : pool one, filter 3, padding 0, stride 2
	p1In = conv1Out;
	p1Width = (conv1Width - 3 + 0)/2 +1;
	p1Width = (conv1Width - 3 + 0)/2 +1;
	p1Out = (unsigned char*)malloc(3 * p1Width * p1Height * sizeof(unsigned char)); //
	MAXpool(p1In, p1Out, 3, 2, 0);

	//fire2 squeeze , 16 kernels, kernel size 1, padding 0, stride 1
	f2SqWidth = p1Width; //1x1 convolution
	f2SqHeight = p1Height;
	f2SqIn= p1Out;
	f2SqOut = (unsigned char*)malloc(3 * f2SqWidth * f2SqHeight * sizeof(unsigned char));
	fltF2Sq = (float*)malloc(3  *  1 * 1 * 16 * sizeof(float));
	randomInit(fltF2Sq, (3 * 1 * 1 * 16), 255);
	conv(f2SqIn, f2SqOut,fltF2Sq, f2SqWidth, f2SqHeight,16,1,0,1); 
	relu(f2SqOut,(3 * f2SqWidth * f2SqHeight)); //relu data size = size of fire2_sqOut

	//fire2 expand 1 x 1:  64 kernels, kernel size 1, padding 0, stride 1
	f2Ex1Width = f2SqWidth; //1x1 convolution
	f2Ex1Height = f2SqHeight;
	f2Ex1In = f2SqOut;
	f2Ex1Out = (unsigned char*)malloc(3 * f2Ex1Width * f2Ex1Height * sizeof(unsigned char));
	fltF2Ex1 = (float*)malloc(3  *  1 * 1 * 64 * sizeof(float));
	randomInit(fltF2Ex1, (3 * 1 * 1 * 64), 255);
	conv(f2Ex1In, f2Ex1Out,fltF2Ex1, f2Ex1Width, f2Ex1Height,64,1,0,1); 

	//fire2 expand 3 x 3:  64 kernels, kernel size 3, padding 0, stride 1
	f2Ex3Width = f2Ex1Width + 3 +1; 
	f2Ex3Height = f2Ex1Height + 3 +1;
	f2Ex3In = f2SqOut;
	f2Ex3Out = (unsigned char*)malloc(3 * f2Ex3Width * f2Ex3Height * sizeof(unsigned char));
	fltF2Ex3 = (float*)malloc(3  *  3 * 3 * 64 * sizeof(float));
	randomInit(fltF2Ex1, (3 * 3 * 3 * 64), 255);
	conv(f2Ex3In, f2Ex3Out,fltF2Ex3, f2Ex3Width, f2Ex3Height,64,3,0,1); 
	relu(f2SqOut,(3 * f2Ex3Width * f2Ex3Height)); 

	imageCount ++;

//	} // end of while, which process 120 images 
	
	// Output..........

	printf ("Done!");

	//clear memory

	//free(image);
	//free(imageLayer);
	//free (filterFire2Sq);
	//free (fire2SqOut);
	//free (filterFire2Expd1);
	//free (fire2Expd1Out);
	//free (filterFire2Expd3);
	//free (fire2Expd3Out);

	return 0;
}
