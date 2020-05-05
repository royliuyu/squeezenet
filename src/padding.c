#include <assert.h>
#include "padding.h"

int PadImageRGB(unsigned char* image, unsigned char* imagePd, \
			 int imgW, int imgH, int dimension, int channel, int totalPadding) 
	{
		int i, j, ch;
		int imgWpadding;
		int imgHpadding;
		int padding;
		padding = totalPadding/2;

		assert(dimension == 1);
		imgWpadding= imgW+totalPadding;
		imgHpadding= imgH+totalPadding;

		for (ch=0; ch<channel; ch++)
		{
			for (i=0; i< imgHpadding;i++)
			{
				for (j=0;j<imgWpadding*3;j+=3)
				{
					//  add o to where rows are the top and bottom OR columes are the head and tail
					if ((i <     padding | i >= (padding+imgH)) | \
						(j <  3* padding | j>= 3*(padding+imgW)) ) 

					{
						imagePd[ch * imgHpadding*imgWpadding*3 + (i*imgWpadding*3+j)]=0; //r
						imagePd[ch * imgHpadding*imgWpadding*3 + (i*imgWpadding*3+j+1)]=0; //g
						imagePd[ch * imgHpadding*imgWpadding*3 + (i*imgWpadding*3+j+2)]=0;  //b
					}

					else
					{
 						imagePd[ch * imgHpadding*imgWpadding*3 + (i*imgWpadding*3+j)]=image[ch * imgW*imgH + 3*(i-padding)*imgW+(j-3*padding)];
						imagePd[ch * imgHpadding*imgWpadding*3 + (i*imgWpadding*3+j+1)]=image[ch * imgW*imgH + 3*(i-padding)*imgW+(j-3*padding)+1];
						imagePd[ch * imgHpadding*imgWpadding*3 + (i*imgWpadding*3+j+2)]=image[ch * imgW*imgH + 3*(i-padding)*imgW+(j-3*padding)+2];
					}
				}
			}		
		}

			return 0;
	}

int PadFtMap(int* input, int* output, 
			 int ftMapW, int ftMapH, int chnl, int dim,  int pdng) // image: the input image, imageLayer:  the output image with padding, channel = 3 for RGB
	{
		int i,j ,d, ch;
		int W;
		int H;


		assert(dim ==1); //feature map's channel=1
		W= ftMapW+2*pdng;
		H= ftMapH+2*pdng;

		for (d=0; d<dim; d++)
		{
			for (ch=0; ch < chnl; ch ++)
			{
				for (i=0; i< H;i++)
				{
					for (j=0;j<W;j++)
					{

						if ((i< pdng | i >= (pdng+ftMapH))| (j < pdng | j>= (pdng+ftMapW))) // add rows of 0 on the top and bottom
						{
							output[d *chnl*H*W + ch*H*W + (i * W + j)]=0; //r
						}
						
						else
						{
							output[d * chnl*H*W + ch*H*W + (i * W + j)]= \
								input[d * chnl*ftMapH*ftMapW + ch * ftMapH*ftMapW + (i-pdng)*ftMapW+(j-pdng)];

						}//end of if
					}
				}				
			}// end of ch loop
		}
		return 0;
	}
