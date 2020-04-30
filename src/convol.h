#ifndef __CONVOL_H__
#define __CONVOL_H__

int ConvRGB(unsigned char* convInput, int* convOutput, float *kernel ,
			  int imgW_padding, int imgH_padding, int kernelNum, int kernelSize, int stride); // RGB convolution, kernels don't merge

int Conv(int* convIn, int* convOut, float *knl,
			  int imgW, int imgH, int dim, int knlNum, int knlSize, int padding, int strd);

int Conv1x1(int* convIn1, int* convOut1, float *knl1,
			   int imgW1, int imgH1, int dim1, int padding1, int knlNum1) ;

#endif