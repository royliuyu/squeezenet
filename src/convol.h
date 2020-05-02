#ifndef __CONVOL_H__
#define __CONVOL_H__

int ConvRGB(unsigned char* convInput, int* convOutput, float *kernel, \
			  int imageW, int imageH, int dimension, int channel, int kernelSize, int padding, int stride) ; // RGB convolution, only support dimension = 1 

int Conv(int* convIn, int* convOut, float *knl, \
			  int imgW, int imgH, int dim, int chn, int knlSize, int pd, int strd) ;

int Conv1x1(int* convIn1, int* convOut1, float *knl1, \
			   int imgW1, int imgH1, int dim1, int chn1, int knlSize1, int pd1, int strd1) ;

#endif