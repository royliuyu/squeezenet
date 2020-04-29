#include <stdlib.h>
#include "convol.h"


int conv(unsigned char* convInput, unsigned char* convOutput, float *kernel ,
			  int imgW_padding, int imgH_padding, int kernelNum, int kernelSize, int padding, int stride)

{
	int i,j,k, row, column;
	int convWidth;
	float r,g,b;
		
	convWidth = (imgW_padding - kernelSize + 2 * padding) / stride + 1;

	r = 0;
	g = 0;
	b = 0;
	for  (k=0; k<kernelNum;k++)
	{
		for (row=0; row < (imgH_padding - padding + 1);row++) //row of image
		{
			for (column=0; column < (imgW_padding *3 - padding +1); column++) //column of image
			{
				for (i = 0; i < kernelSize; i++) // row of kernel
				{
					for (j = 0; j < kernelSize; j++) //column of kernel
					{
						r += kernel[i*kernelSize + j] * convInput[3 * (row + i) * imgW_padding + column + j]; //R
						g += kernel[i*kernelSize + j] * convInput[3 * (row + i) * imgW_padding + column + j + 1];//G
						b += kernel[i*kernelSize + j] * convInput[3 * (row + i) * imgW_padding + column + j + 2]; //B
					}
				}
				convOutput[row * convWidth + column] = r;
				convOutput[row * convWidth + column + 1] = g;
				convOutput[row * convWidth + column + 2] = b;
			}
		}
	}
	return 0;
}