
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
	unsigned char* conv1OutChar;

	//maxPool 1
	int* p1In;
	int* p1Out; //max Pool one
	int p1Width, p1Height;
	unsigned char* p1OutChar;

	//fire 2
	int* f2SqIn;
	int* f2SqOut;
	int f2SqWidth, f2SqHeight;
	float* fltF2Sq;
	unsigned char* f2SqOutChar;

	int f2Ex1Width,f2Ex1Height;
	int* f2Ex1In;
	int* f2Ex1Out;
	float * fltF2Ex1;
	unsigned char* f2Ex1OutChar;

	int f2Ex3Width,f2Ex3Height;
	int* f2Ex3In;
	int* f2Ex3Out;
	float * fltF2Ex3;
	unsigned char* f2Ex3OutChar;

	int* f2Out;
	unsigned char* f2OutChar;

	int i;