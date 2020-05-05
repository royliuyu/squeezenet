#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "convol.h"
#include "defines.h"
#include "padding.h"


//for revoluting 227*227*3 img conv 96*7*7*3 filter 
int ConvRGB(unsigned char* convInput, int* convOutput, float *kernel, \
			  int imageW, int imageH, int channel, int dimension, int kernelSize, int padding, int stride) //imgW_padding counted padding already

{
	int i,j,ch, row, column, convRow, convCol;
	int convWidth,convHeight;
	int sum;
	int r,g,b,a, add;

	assert(padding == 0);
	assert(dimension == 1);

	convWidth = (imageW - kernelSize) / stride + 1; //image with padding
	convHeight = (imageW - kernelSize) / stride + 1;

	r = 0;
	g = 0;
	b = 0;
	for  (ch=0; ch<channel;ch++)
	{
		convRow = 0;
		for (row=0; row < (imageH - kernelSize + 1);row+=stride) //row of image
		{
			convCol = 0;
			for (column=0; column < (imageW - kernelSize + 1)*3; column+=stride*3) //column of image
			{
				r=0;
				g=0;
				b=0;
				sum = 0;
				for (i = 0; i < kernelSize; i++) // row of kernel
				{
					for (j = 0; j < kernelSize; j++) //column of kernel
					{
						r = kernel[ch*kernelSize*kernelSize*3 + (i*kernelSize*3 + j)]   * convInput[3 * (row + i) * imageW + column + j]; //R
						g = kernel[ch*kernelSize*kernelSize*3 + (i*kernelSize*3 + j+1)] * convInput[3 * (row + i) * imageW + column + j + 1];//G
						b = kernel[ch*kernelSize*kernelSize*3 + (i*kernelSize*3 + j+2)] * convInput[3 * (row + i) * imageW + column + j + 2]; //B
						sum += (r+g+b);
					}
				}// end of i loop (filter iteration)
				convOutput[ch* convWidth * convHeight + convRow * convWidth + convCol] = sum;
				add=ch* convWidth * convHeight + convRow * convWidth + convCol;
				a=convOutput[ch* convWidth * convHeight + convRow * convWidth + convCol] ;
				convCol++;
			}
			convRow++;
		}//end of row loop, (image iteration)
	}
	return 0;
}

int Conv(int* convIn, int* convOut, float *knl, \
			  int imgW, int imgH, int chn, int dim, int knlSize, int pd, int strd) // dimensions are (width, height, channel)

{

	int i,j,ch,d,r,c; //d: dimension, ch: channel/knlNum, r : row,c : colume
	int convWidth,convHeight, convRow, convCol;
	int sumImg;
	int* newConvIn;

	newConvIn = (int*)malloc(chn*(imgW+2*pd)*(imgH+2*pd)*sizeof(int));
	if (pd >0) //padding 
	{
		PadFtMap(convIn, newConvIn, imgW,imgH, chn, 1, pd); //feature map dimension = 1
		imgW += 2*pd;
		imgH += 2*pd;
	}
	else
		for (i=0;i<imgW*imgH; i++)
			{
				  newConvIn[i] = convIn[i];
			}
		
	convWidth = (imgW - knlSize) / strd + 1; //image with padding
	convHeight = (imgH - knlSize) / strd + 1; 

	for (d=0; d<dim; d++)
	{
		convRow = 0;
		for (r=0; r < (imgH - knlSize + 1); r+=strd) //row of image
		{
			convCol = 0;
			for (c=0; c < (imgW - knlSize + 1); c +=strd) //column of image
			{
				sumImg = 0;
				for  (ch=0; ch<chn;ch++) //channels
				{
					for (i = 0; i < knlSize; i++) // row of kernel
					{
						for (j = 0; j < knlSize; j++) //column of kernel
						{
							sumImg += knl[d*chn*knlSize*knlSize+(ch*knlSize*knlSize +(i*knlSize + j))] * \
								newConvIn[ch*imgW*imgH+((r + i) * imgW + c + j)]; 
						}
					}

				}// end of ch (channel merge)
				convOut[d * convWidth * convHeight + convRow * convWidth + convCol] =sumImg;
				convOut[d * convWidth * convHeight + convRow * convWidth + convCol] = \
					MAX(convOut[d * convWidth * convHeight + convRow * convWidth + convCol],0);//relu
				convCol++;
			}
			convRow ++;
		} // end of image row
	}// end of dimension
	return 0;
	free(newConvIn);
	newConvIn = NULL;
}


int Conv1x1(int* convIn1, int* convOut1, float *knl1, \
			   int imgW1, int imgH1, int chn1, int dim1, int knlSize1, int pd1, int strd1) //imgW : image width with padding£¬ merge channel of kernels

{
	int ch,d,r,c; //d: dimension, k: channel/knlNum, r : row,c : colume
	int convWidth,convHeight,convRow, convCol;
	int sumImg;

	assert(pd1 ==0); // padding must to be 0
	assert(knlSize1 ==1);
	assert(strd1 ==1 );
		
	convWidth = (imgW1 - 1) / 1 + 1; //image with padding
	convHeight = (imgH1 -1) / 1 + 1;

	for (d=0; d<dim1; d++)
	{
		convRow =0;
		for (r=0; r < (imgH1 - 1 + 1); r+=strd1) //row of image
		{
			convCol = 0;
			for (c=0; c < (imgW1 - 1 + 1); c+=strd1) //column of image
			{
				sumImg =0;
				for  (ch=0; ch< chn1;ch++) //channels
				{
					sumImg += knl1[d*chn1+ch] * convIn1[ch*imgW1*imgH1+ (r * imgW1 + c )]; 
				}// end of ch (channel merge)
				convOut1[d * convWidth * convHeight + convRow * convWidth + convCol] = sumImg;
				convOut1[d * convWidth * convHeight + convRow * convWidth + convCol] = \
					MAX(convOut1[d * convWidth * convHeight + convRow * convWidth + convCol],0); //relu
				convCol++;
			}
			convRow ++;
		} // end of image row
	}// end of dimension
	return 0;
}