#include <assert.h>
#include "padding.h"

int PadImageRGB(unsigned char* image, unsigned char* imageLayer, \
			 int imageWidth, int imageHeight, int dimension, int channel, int totalPadding) 
	{
		int i, j, ch;
		int imgW;
		int imgH;

		assert(dimension == 1);

		imgW= imageWidth+totalPadding;
		imgH= imageHeight+totalPadding;

		for (ch=0; ch<channel; ch++)
		{
			for (i=0; i< imgH;i++)
			{
				for (j=0;j<imgW*3;j++)
				{
					//  add o to where rows are the top and bottom OR columes are the head and tail
					if ((i < totalPadding/2 | i >= (totalPadding/2+imageHeight)) | \
						(j <  3* totalPadding/2 | j>= 3*(totalPadding/2+imageWidth)) ) 

					{
						imageLayer[ch * imgH*imgW*3 + (i*imgW*3+j)]=0; //r
						imageLayer[ch * imgH*imgW*3 + (i*imgW*3+j+1)]=0; //g
						imageLayer[ch * imgH*imgW*3 + (i*imgW*3+j+2)]=0;  //b
					}

					else
					{
						imageLayer[ch * imgH*imgW*3 + (i*imgW*3+j)]=image[ch * imageWidth*imageHeight + (i-totalPadding/2)*imageWidth+(j-totalPadding/2)];
						imageLayer[ch * imgH*imgW*3 + (i*imgW*3+j+1)]=image[ch * imageWidth*imageHeight + (i-totalPadding/2)*imageWidth+(j-totalPadding/2)+1];
						imageLayer[ch * imgH*imgW*3 + (i*imgW*3+j+2)]=image[ch * imageWidth*imageHeight + (i-totalPadding/2)*imageWidth+(j-totalPadding/2)+2];
					}
				}
			}		
		}

			return 0;
	}
int PadFtMap(int* input, int* output, 
			 int ftMapWidth, int ftMapHeight, int dim, int chnl, int pdng) // image: the input image, imageLayer:  the output image with padding, channel = 3 for RGB
	{
		int i,j ,d, k;
		int W;
		int H;
		W= ftMapWidth+2*pdng;
		H= ftMapHeight+2*pdng;

		for (d=0; d<dim; d++)
		{
			for (k=0; k< chnl; k++)
			{
				for (i=0; i< H;i++)
				{
					for (j=0;j<W;j++)
					{

						if (i< pdng | i >= (pdng+ftMapHeight)) // add rows of 0 on the top and bottom
						{
							output[d * chnl*H*W + k*H*W + (i * W + j)]=0; //r
						}
						else if (j < pdng | j>= (pdng+ftMapWidth)) // add columes of 0 on the head and tail
						{
							output[d * chnl*H*W + k*H*W + (i * W + j)]=0;
						}
						else
						{
							output[d * chnl*H*W + k*H*W + (i * W + j)]= \
								input[d * chnl*ftMapHeight*ftMapWidth + k * ftMapHeight*ftMapWidth + (i-pdng)*ftMapWidth+(j-pdng)];
						}//end of if
					}
				}				
			}// end of k loop
		}
		return 0;
	}
