#ifndef __CONVOL_H__
#define __CONVOL_H__

int convolute(float *convInput, float *kernel , float * convOutput, int imageWidth, int imageHeight, int kernelNum, int kernelSize, int padding, int stride);

#endif