#include "relu.h"
int relu(float* data, int reluLen) {
int i;

#pragma clang loop unroll(disable)    
for (i = 1; i < reluLen; i++) {
	data[i] = MAX(data[i],0);
    }
	return 0; // if success
}