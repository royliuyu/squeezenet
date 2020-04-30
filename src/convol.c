#include <stdlib.h>
#include <stdio.h>
#include "convol.h"
#include "defines.h"
#include "padding.h"

//for revoluting 227*227*3
int ConvRGB(unsigned char* convInput, int* convOutput, float *kernel ,
			  int imgW_padding, int imgH_padding, int kernelNum, int kernelSize, int stride) //imgW_padding counted padding already

{
	int i,j,k, row, column;
	int convWidth,convHeight;
	int sum;
	float r,g,b;
		
	convWidth = (imgW_padding - kernelSize) / stride + 1; //image with padding

	r = 0;
	g = 0;
	b = 0;
	for  (k=0; k<kernelNum;k++)
	{
		for (row=0; row <= (imgH_padding - kernelSize);row+=stride) //row of image
		{
			for (column=0; column <= (imgW_padding - kernelSize)*3; column+=stride*3) //column of image
			{
				for (i = 0; i < kernelSize; i++) // row of kernel
				{
					for (j = 0; j < kernelSize; j++) //column of kernel
					{
						r += kernel[i*kernelSize + j] * convInput[3 * (row + i) * imgW_padding + column + j]; //R
						g += kernel[i*kernelSize + j] * convInput[3 * (row + i) * imgW_padding + column + j + 1];//G
						b += kernel[i*kernelSize + j] * convInput[3 * (row + i) * imgW_padding + column + j + 2]; //B
						sum = r+g+b;
					}
				}
				convOutput[(row * convWidth + column)*k] = sum;

			}
		}
	}
	return 0;
}

int Conv(int* convIn, int* convOut, float *knl,
			  int imgW, int imgH, int dim, int knlNum, int knlSize, int padding, int strd) // knlNum channel of kernels are merged

{

	int i,j,k,d,r,c; //d: dimension, k: channel/knlNum, r : row,c : colume
	int convWidth,convHeight;
	int sumImg, sumChnl;
	int* newConvIn;

	if (padding >0) //padding
		{
			newConvIn = (int*)malloc((imgW+2*padding)*(imgH+2*padding)*sizeof(int));
			PadFtMap(convIn, newConvIn, imgW,imgH, dim, knlNum, padding);
			imgW += padding;
			imgH += padding;
			free(convIn);
			convIn = newConvIn;
			newConvIn = NULL;
		}
		
	convWidth = (imgW - knlSize) / strd + 1; //image with padding

	for (d=0; d<dim; d++)
	{
		for (r=0; r < (imgH - knlSize + 1); r+=strd) //row of image
		{
			for (c=0; c < (imgW - knlSize + 1); c +=strd) //column of image
			{
				for  (k=0; k<knlNum;k++) //channels
				{
					for (i = 0; i < knlSize; i++) // row of kernel
					{
						for (j = 0; j < knlSize; j++) //column of kernel
						{
							sumImg= knl[d*dim+(k*knlSize +(i*knlSize + j))] * convIn[k*knlSize+((r + i) * imgW + c + j)]; 
						}
					}
					convOut[d*dim+(r * convWidth + c)] += sumImg;
				}// end of k (channel merge)
				convOut[d*dim+(r * convWidth + c)] = MAX(convOut[d*dim+(r * convWidth + c)],0);//relu
			}
		} // end of image row
	}// end of dimension
	return 0;
}

int Conv1x1(int* convIn1, int* convOut1, float *knl1,
			   int imgW1, int imgH1, int dim1, int padding1, int knlNum1) //imgW : image width with padding£¬ merge channel of kernels

{
	int i,j,k,d,r,c; //d: dimension, k: channel/knlNum, r : row,c : colume
	int convWidth,convHeight;
	int sumImg, sumChnl;

		if (padding1 >0) //padding
		{
			printf("1 x 1 convolution doesn't apply padding >0  \n");
			return 1;
		}
		
	convWidth = (imgW1 - 1) / 1 + 1; //image with padding

	for (d=0; d<dim1; d++)
	{
		for (r=0; r < (imgH1 - 1 + 1); r++) //row of image
		{
			for (c=0; c < (imgW1 - 1 + 1); c ++) //column of image
			{
				for  (k=0; k<knlNum1;k++) //channels
				{
					sumImg= knl1[d*dim1+k] * convIn1[k+((r ) * imgW1 + c )]; 

					convOut1[d*dim1+(r * convWidth + c)] += sumImg;
				}// end of k (channel merge)
				convOut1[d*dim1+(r * convWidth + c)] = MAX(convOut1[d*dim1+(r * convWidth + c)],0); //relu
			}
		} // end of image row
	}// end of dimension
	return 0;
}