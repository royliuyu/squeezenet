
	//image process to imageLayer, transfer 224x224 to 227x227
	int imgWidth, imgHeight;
	int imageCount;
	unsigned char* image;
	unsigned char* imgLayer;
	int imgLayerWidth, imgLayerHeight;

	//convolution 1
	float* fltConv1;
	unsigned char* conv1In;
	int* conv1Out;
	int conv1Width, conv1Height;

	//maxPool 1
	int* p1In;
	int* p1Out; //max Pool one
	int p1Width, p1Height;

	//fire 2
	int* f2SqIn;
	int* f2SqOut;
	int f2SqWidth, f2SqHeight;
	float* fltF2Sq;

	int f2Ex1Width,f2Ex1Height;
	int* f2Ex1In;
	int* f2Ex1Out;
	float * fltF2Ex1;

	int f2Ex3Width,f2Ex3Height;
	int* f2Ex3In;
	int* f2Ex3Out;
	float * fltF2Ex3;

	int* f2Out;
