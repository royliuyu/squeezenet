//**********************************************************************
//                                                                     *
//               University Of North Carolina Charlotte                *
//                                                                     *
//Program: main for squeezenet                                         *
//Description: This program is to do squeezenet                        *
//                                                                     *
//File Name: main.cpp                                                  *
//File Version: 1.0                                                    *
//Author: Roy Liu                                                      *
//                                                                     *
//                                                                     *
//Input file: images/viptraffic0.ppm ...  images/viptraffic119.ppm     *
//Output file: feature maps in ppm                                     *
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



int main(int argc, char** argv) {
	
	//read image
	imgWidth = IMAGE_WIDTH ; //image is color's with RGB
	imgHeight = IMAGE_HEIGHT;
	image = (unsigned char*)malloc(imgHeight * imgWidth * 3 * sizeof(unsigned char));
	ReadPPM( "../images/uncc.ppm", image, imgWidth, imgHeight);
	//printImgRGB(image,imgWidth, imgHeight,1); 	system("pause");//for debuging
	SavePPM(image, "../featuremaps/original_image.ppm",imgWidth,imgHeight); 
	 
	//image processing, add padding =3 for processing, RGB. 
	//In 224*224*3, Out 227*227*3, filter: none
	imgLayerWidth = imgWidth+3;
	imgLayerHeight = imgHeight+3;
	imgLayer = (unsigned char*)malloc(imgLayerWidth * imgLayerHeight * 3 * sizeof(unsigned char));
	PadImageRGB(image,imgLayer, imgWidth, imgHeight, 1, 1, 3); //transfer size from 224x224 to 227x227 pad a image , add totally 3 pad, RGB 
	//printImgRGB(imgLayer,imgLayerWidth, imgLayerHeight,1); 	system("pause");//for debuging
	SavePPM(imgLayer, "../featuremaps/imglayer.ppm",imgLayerWidth,imgLayerHeight);   //create ppm file of feature map
	free(image);
	image = NULL;

	//convolution 1, 96 kernals, kernel size 7, padding 0, stride 2
	//In 227*227*3, filter 7*7*96 -> Out 111*111*96£¬
	conv1In = imgLayer;
	conv1Width = (imgLayerWidth - 7 +0)/2+1; //kernel 7*7, padding 0, stride 2
	conv1Height = (imgLayerHeight - 7 +0)/2+1; 
	conv1Out =	(int *)malloc(96 * conv1Width * conv1Height *sizeof(int));
	fltConv1 = (float*)malloc(3  *  7 * 7 * 96 * sizeof(float));
	RandomInit(fltConv1, (3* 7 * 7 * 96), 255);
	ConvRGB(conv1In,conv1Out,fltConv1,imgLayerWidth,imgLayerHeight,96,1, 7, 0, 2);  //imgLayerWidth is with padding, dimensions are (width, height, channel)
	//printFtMapInt(conv1Out, conv1Width, conv1Height, 96); system("pause"); //for debugging
	conv1OutChar = (unsigned char*)malloc(conv1Width * conv1Height * 96 * sizeof(unsigned char));
	NormFtMap(conv1Out, conv1OutChar, conv1Width, conv1Height, 1, 96, 7*7*3*255); //normalization for visualization the feature map, 
	BatchSavePPM (conv1OutChar, "../featuremaps/conv1", conv1Width, conv1Height, 96);
	free(imgLayer);
	conv1In = NULL;
	imgLayer = NULL;
	free(conv1OutChar);
	conv1OutChar = NULL;

	//max pool, pool1 : pool one, filter 3, padding 0, stride 2
	//In 111*111*96, Out: 55*55*96, filter: none
	p1In = conv1Out;
	p1Width = (conv1Width - 3 + 0)/2 +1; //kernel size to be 55
	p1Height = (conv1Height - 3 + 0)/2 +1;
	p1Out = (int*)malloc(p1Width * p1Height * 96 * sizeof(int)); // 55*55*96
	MAXpool(p1In, p1Out,conv1Width, conv1Height, 1, 96, 3, 0, 2 ); // feature map's channel =1
	p1OutChar = (unsigned char*)malloc(p1Width * p1Height * 96 * sizeof(unsigned char));
	NormFtMap(p1Out, p1OutChar, p1Width, p1Height,  1, 96, 7*7*3*255);
	BatchSavePPM (p1OutChar, "../featuremaps/p1", p1Width, p1Height, 96);
	free (conv1Out);
	p1In = NULL;
	conv1Out = NULL;
	free(p1OutChar);
	p1OutChar = NULL;

	//fire2 squeeze , 16 dims, 128 kernels/dim, kernel size 1, padding 0, stride 1
	//In 55*55*96 , filter 1*1*96*16 -> out 55*55*16
	f2SqWidth = p1Width; //1x1 convolution
	f2SqHeight = p1Height;
	f2SqIn= p1Out;
	f2SqOut = (int*)malloc(16 * f2SqWidth * f2SqHeight * sizeof(int));
	fltF2Sq = (float*)malloc(1 * 1 * 96 * 16 * sizeof(float));
	RandomInit(fltF2Sq, (1 * 1 * 96 * 16 ), 255);
	Conv1x1(f2SqIn, f2SqOut,fltF2Sq, f2SqWidth, f2SqHeight,96, 16, 1, 0, 1);
	f2SqOutChar = (unsigned char*)malloc(16 * f2SqWidth * f2SqHeight * sizeof(unsigned char));
	NormFtMap(f2SqOut, f2SqOutChar, f2SqWidth, f2SqHeight,  1, 16, 7*7*96*255); //7*7*3*96*255
	BatchSavePPM (f2SqOutChar, "../featuremaps/f2sq", f2SqWidth, f2SqHeight, 16);
	free (p1Out);
	free (f2SqOutChar);
	f2SqIn = NULL;
	p1Out = NULL;
	f2SqOutChar = NULL;

	//fire2 expand 1 x 1:  64 dims, 16 kernels/dim, kernel size 1, padding 0, stride 1
	//in 55*55*64, filter 1*1*64*16 -> out 55*55*64
	f2Ex1Width = f2SqWidth;
	f2Ex1Height = f2SqHeight;
	f2Ex1In = f2SqOut; // connect f2 squeeze output
	f2Ex1Out = (int*)malloc(64* f2Ex1Width * f2Ex1Height * sizeof(int));
	fltF2Ex1 = (float*)malloc(64 * 16 *  1 * 1 * sizeof(float));
	RandomInit(fltF2Ex1, (64 * 16 * 1 * 1), 255);
	Conv1x1(f2Ex1In, f2Ex1Out,fltF2Ex1, f2Ex1Width, f2Ex1Height,16,64,1,0,1); 
	f2Ex1OutChar = (unsigned char*)malloc(64 * f2Ex1Width * f2Ex1Height * sizeof(unsigned char));
	NormFtMap(f2Ex1Out, f2Ex1OutChar, f2Ex1Width, f2Ex1Height, 1, 64, 7*7*16*96*255); //7*7*3*96*16*255
	BatchSavePPM (f2Ex1OutChar, "../featuremaps/f2Ex1", f2Ex1Width, f2Ex1Height, 64);
	free(f2Ex1OutChar);
	f2Ex1In = NULL;
	f2Ex1OutChar = NULL;

	//fire2 expand 3 x 3:  64 dims, 16 kernels/dim, kernel size 3, padding 0, stride 1
	//in 55*55*64, filter 3*3*64*16 -> out 55*55*64
	f2Ex3Width = f2SqWidth; 
	f2Ex3Height = f2SqHeight;
	f2Ex3In = f2SqOut;
	f2Ex3Out = (int*)malloc(64 * f2Ex3Width * f2Ex3Height * sizeof(int));
	fltF2Ex3 = (float*)malloc(64 * 16 * 3  * 3 *  sizeof(float));
	RandomInit(fltF2Ex3, (64 * 16 * 3 * 3), 255);
	Conv(f2Ex3In, f2Ex3Out,fltF2Ex3, f2Ex3Width, f2Ex3Height, 16,64, 3, 1,1); //need add padding =1
 	f2Ex3OutChar = (unsigned char*)malloc(64 * f2Ex3Width * f2Ex3Height * sizeof(unsigned char));
	NormFtMap(f2Ex3Out, f2Ex3OutChar, f2Ex3Width, f2Ex3Height, 1, 64, 7*7*3*96*16*3*255); //original is 7*7*3*96*16*3*3*255
	BatchSavePPM (f2Ex3OutChar, "../featuremaps/f2Ex3", f2Ex1Width, f2Ex1Height, 64);
	free(f2SqOut);
	free(f2Ex3OutChar);
	f2Ex3In = NULL; 
	f2SqOut = NULL;
	f2Ex3OutChar = NULL;

	//Concat fire2 expand 1 and expand 3
	f2Out = (int*) malloc(f2Ex1Width*f2Ex1Height*128*sizeof(int));
	if (f2Ex1Width != f2Ex3Width | f2Ex1Height != f2Ex3Height) 
	{
		printf("feature map's size shall be the same,  concat failed !\n");
		return 1;
	}
	else Concat (f2Out, f2Ex1Out, f2Ex3Out,55,55,64,64); 
	free(f2Ex1Out);
	free(f2Ex3Out);
	f2Ex1Out = NULL;
	f2Ex3Out = NULL;

	/* 
	fire 3 
	
	....

	avgPool10
	*/


	printf ("Job done! \n");
	system("pause");

	//clear memory
	free(fltConv1);
	free(fltF2Sq);
	free (fltF2Ex1);
	free (fltF2Ex3);
	fltConv1=NULL;
	fltF2Sq = NULL;
	fltF2Ex1 = NULL;
	fltF2Ex3 = NULL;

	return 0;
}
