#include <stdlib.h>
#include <assert.h>
#include "padding.h"
#include "defines.h"

int MAXpool(int* input, int* output, int imgW, int imgH, int channel,int dim,  int knlSize, int padding , int strd) //dimensions are (width, height, channel)
{
	int* newFtMap;
	int r, c, d, i, j;
	int pool;
	int poolW, poolH, poolRow, poolCol;

	assert(channel == 1);
	pool = 0;
	if (padding >0) //padding
	{
		newFtMap = (int*)malloc((imgW+2*padding)*(imgH+2*padding)*sizeof(int));
		PadFtMap(input, newFtMap, imgW, imgH, dim, channel, padding);
		imgW += padding;
		imgH += padding;
		free(input);
		input = newFtMap;
		newFtMap = NULL;
	}
	poolW = (imgW - knlSize) / strd + 1; //image with padding	
	poolH = (imgH - knlSize) / strd + 1;
	for (d=0; d<dim; d++)
	{
		poolRow = 0;
		for (r=0; r < (imgH - knlSize + 1); r+=strd) //row of image
		{
			poolCol = 0;
			for (c=0; c < (imgW - knlSize + 1); c +=strd) //column of image
			{
				pool = 0;
				for (i = 0; i < knlSize; i++) // row of kernel
				{
					for (j = 0; j < knlSize; j++) //column of kernel
					{
						pool= MAX(input[d*imgW * imgH +((r + i) * imgW + c + j)],pool);

					}
				}// end of j loop
				output[d*poolW*poolH + poolRow * poolW + poolCol] = pool;
				poolCol++;
			}
			poolRow++;
		} // end of image row
	}// end of dimension

	return 0;
}