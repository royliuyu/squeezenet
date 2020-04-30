#include <stdlib.h>
#include "padding.h"
#include "defines.h"
int MAXpool(int* input, int* output, int imgW, int imgH, int knlNum, int knlSize, int padding , int strd)
{
	int* newFtMap;
	int r, c, k, i, j;
	int pool;
	int poolWidth;
	pool = 0;
	if (padding >0) //padding
	{
		newFtMap = (int*)malloc((imgW+2*padding)*(imgH+2*padding)*sizeof(int));
		PadFtMap(input, newFtMap, imgW, imgH,1, knlNum, padding);
		imgW += padding;
		imgH += padding;
		free(input);
		input = newFtMap;
		newFtMap = NULL;
	}
	poolWidth = (imgW - knlSize) / strd + 1; //image with padding	
	for (k=0; k<knlNum; k++)
	{
		for (r=0; r < (imgH - knlSize + 1); r+=strd) //row of image
		{
			for (c=0; c < (imgW - knlSize + 1); c +=strd) //column of image
			{				
				for (i = 0; i < knlSize; i++) // row of kernel
				{
					for (j = 0; j < knlSize; j++) //column of kernel
					{
						pool= MAX(input[k*knlSize+((r + i) * imgW + c + j)],pool);						
					}
				}
				output[k*knlNum+(r * poolWidth + c)] = pool;
			}
		} // end of image row
	}// end of dimension

	return 0;
}