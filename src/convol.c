#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "convol.h"
#include "defines.h"
#include "padding.h"


//for revoluting 227*227*3
int ConvRGB(unsigned char* convInput, int* convOutput, float *kernel, \
			  int imageW, int imageH, int dimension, int channel, int kernelSize, int padding, int stride) //imgW_padding counted padding already

{
	int i,j,ch, row, column;
	int convWidth,convHeight;
	int sum;
	float r,g,b;

	assert(padding == 0);
	assert(dimension == 1);

	convWidth = (imageW - kernelSize) / stride + 1; //image with padding
	convHeight = (imageW - kernelSize) / stride + 1;

	r = 0;
	g = 0;
	b = 0;
	for  (ch=0; ch<channel;ch++)
	{
		for (row=0; row < (imageH - kernelSize + 1);row+=stride) //row of image
		{
			for (column=0; column < (imageW - kernelSize + 1)*3; column+=stride*3) //column of image
			{
				for (i = 0; i < kernelSize; i++) // row of kernel
				{
					for (j = 0; j < kernelSize; j++) //column of kernel
					{
						r += kernel[i*kernelSize*3 + j] * convInput[ch* convWidth * convHeight + (3 * (row + i) * imageW + column + j)]; //R
						g += kernel[i*kernelSize*3 + j+1] * convInput[ch* convWidth * convHeight + (3 * (row + i) * imageW + column + j + 1)];//G
						b += kernel[i*kernelSize*3 + j+2] * convInput[ch* convWidth * convHeight + (3 * (row + i) * imageW + column + j + 2)]; //B
						sum = r+g+b;
					}
				}
				convOutput[ch* convWidth * convHeight + (row * convWidth + column)] = sum;

			}
		}
	}
	return 0;
}

int Conv(int* convIn, int* convOut, float *knl, \
			  int imgW, int imgH, int dim, int chn, int knlSize, int pd, int strd) // knlNum channel of kernels are merged

{

	int i,j,ch,d,r,c; //d: dimension, k: channel/knlNum, r : row,c : colume
	int convWidth,convHeight;
	int sumImg, sumChnl;
	int* newConvIn;

	if (pd >0) //padding
		{
			newConvIn = (int*)malloc((imgW+2*pd)*(imgH+2*pd)*sizeof(int));
			PadFtMap(convIn, newConvIn, imgW,imgH, dim, chn, pd);
			imgW += pd;
			imgH += pd;
			free(convIn);
			convIn = newConvIn;
			newConvIn = NULL;
		}
		
	convWidth = (imgW - knlSize) / strd + 1; //image with padding
	convHeight = (imgH - knlSize) / strd + 1; 

	for (d=0; d<dim; d++)
	{
		for (r=0; r < (imgH - knlSize + 1); r+=strd) //row of image
		{
			for (c=0; c < (imgW - knlSize + 1); c +=strd) //column of image
			{
				convOut[(d+1)*(r * convWidth + c)] = 0;
				for  (ch=0; ch<chn;ch++) //channels
				{
					for (i = 0; i < knlSize; i++) // row of kernel
					{
						for (j = 0; j < knlSize; j++) //column of kernel
						{
							sumImg= knl[d*chn*knlSize*knlSize+(ch*knlSize*knlSize +(i*knlSize + j))] * convIn[ch*imgW*imgH+((r + i) * imgW + c + j)]; 
						}
					}
					convOut[d * convWidth * convHeight + (r*convWidth +c)] +=sumImg;
				}// end of k (channel merge)
				convOut[d * convWidth * convHeight + (r*convWidth +c)] = MAX(convOut[d * convWidth * convHeight + (r*convWidth +c)],0);//relu
			}
		} // end of image row
	}// end of dimension
	return 0;
}

int Conv1x1(int* convIn1, int* convOut1, float *knl1, \
			   int imgW1, int imgH1, int dim1, int chn1, int knlSize1, int pd1, int strd1) //imgW : image width with padding£¬ merge channel of kernels

{
	int i,j,ch,d,r,c; //d: dimension, k: channel/knlNum, r : row,c : colume
	int convWidth,convHeight;
	int sumImg, sumChnl;

	assert(pd1 ==0); // padding must to be 0
	assert(knlSize1 ==1);
	assert(strd1 ==1 );
		
	convWidth = (imgW1 - 1) / 1 + 1; //image with padding
	convHeight = (imgH1 -1) / 1 + 1;

	for (d=0; d<dim1; d++)
	{
		for (r=0; r < (imgH1 - 1 + 1); r++) //row of image
		{
			for (c=0; c < (imgW1 - 1 + 1); c ++) //column of image
			{
				convOut1[d*dim1+(r * convWidth + c)]=0;
				for  (ch=0; ch< chn1;ch++) //channels
				{
					sumImg= knl1[d*chn1+ch] * convIn1[ch*imgW1*imgH1+ (r * imgW1 + c )]; 
					convOut1[d * convWidth * convHeight + (r * convWidth + c)] += sumImg;
				}// end of ch (channel merge)
				convOut1[d * convWidth * convHeight + (r * convWidth + c)] = MAX(convOut1[d * convWidth * convHeight + (r * convWidth + c)],0); //relu
			}
		} // end of image row
	}// end of dimension
	return 0;
}