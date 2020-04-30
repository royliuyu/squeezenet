#include "padding.h"

int PadImageRGB(unsigned char* image, unsigned char* imageLayer, 
			 int imageWidth, int imageHeight, int totalPadding) 
	{
		int i,j;
		int imgW;
		int imgH;
		imgW= imageWidth+totalPadding;
		imgH= imageHeight+totalPadding;

		for (i=0; i< imgH;i++)
			for (j=0;j<imgW*3;j++)
			{

				if (i< totalPadding/2 | i >= (totalPadding/2+imageHeight)) // add rows of 0 on the top and bottom
				{
					imageLayer[i*imgW*3+j]=0; //r
					imageLayer[i*imgW*3+j+1]=0; //g
					imageLayer[i*imgW*3+j+2]=0;  //b

				}
				else if (j <  3* totalPadding/2 | j>= 3*(totalPadding/2+imageWidth)) // add columes of 0 on the head and tail
				{
					imageLayer[i*imgW*3+j]=0;
					imageLayer[i*imgW*3+j+1]=0;
					imageLayer[i*imgW*3+j+2]=0;
				}
				else
				{
					imageLayer[i*imgW*3+j]=image[(i-totalPadding/2)*imageWidth+(j-totalPadding/2)];
					imageLayer[i*imgW*3+j+1]=image[(i-totalPadding/2)*imageWidth+(j-totalPadding/2)+1];
					imageLayer[i*imgW*3+j+2]=image[(i-totalPadding/2)*imageWidth+(j-totalPadding/2)+2];
				}
		return 0;
			}
	}
int PadFtMap(int* input, int* output, 
			 int ftMapWidth, int ftMapHeight, int dim, int kernelNum, int padding) // image: the input image, imageLayer:  the output image with padding, channel = 3 for RGB
	{
		int i,j;
		int W;
		int H;
		W= ftMapWidth+2*padding;
		H= ftMapHeight+2*padding;

		for (i=0; i< H;i++)
			for (j=0;j<W;j++)
			{

				if (i< padding | i >= (padding+ftMapHeight)) // add rows of 0 on the top and bottom
				{
					output[i*W+j]=0; //r
				}
				else if (j <padding | j>= (padding+ftMapWidth)) // add columes of 0 on the head and tail
				{
					output[i*W+j]=0;
				}
				else
				{
					output[i*W+j]=input[(i-padding)*ftMapWidth+(j-padding)];

				}
		return 0;
			}
	}
