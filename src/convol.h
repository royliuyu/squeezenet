#ifndef __CONVOL_H__
#define __CONVOL_H__

int conv(unsigned char* convInput, unsigned char* convOutput, float *kernel ,
			  int imageWidth, int imageHeight, int kernelNum, int kernelSize, int padding, int stride);

#endif