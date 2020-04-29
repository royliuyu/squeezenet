#include "imglayer.h"

int LayerImg(unsigned char* image, unsigned char* imageLayer, 
			 int imgWidth, int imgHeight, int padding) // image: the input image, imageLayer:  the output image with padding, channel = 3 for RGB
	{
		int i,j;
		int imgW;
		int imgH;
		imgW= imgWidth+adder;
		imgH= imageHeight+padding;

		for (i=0; i< imgH;i++)
			for (j=0;j<imgW*3;j++)
			{

				if (i< padding/2 | i >= (padding/2+imageHeight)) // add rows of 0 on the top and bottom
				{
					imageLayer[i*imgW*3+j]=0; //r
					imageLayer[i*imgW*3+j+1]=0; //g
					imageLayer[i*imgW*3+j+2]=0;  //b

				}
				else if (j <  3* padding/2 | j>= 3*(padding/2+imageWidth)) // add columes of 0 on the head and tail
				{
					imageLayer[i*imgW*3+j]=0;
					imageLayer[i*imgW*3+j+1]=0;
					imageLayer[i*imgW*3+j+2]=0;
				}
				else
				{
					imageLayer[i*imgW*3+j]=image[(i-padding/2)*imageWidth+(j-padding/2)];
					imageLayer[i*imgW*3+j+1]=image[(i-padding/2)*imageWidth+(j-padding/2)+1];
					imageLayer[i*imgW*3+j+2]=image[(i-padding/2)*imageWidth+(j-padding/2)+2];
				}
		return 0;
	}