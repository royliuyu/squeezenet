
int Concat (int* output, int* source1, int* source2, int imgWidth, int imgHeight, int dim1, int dim2)
{
	int dim,i,j;
	for (dim=0; dim<dim1; dim++)
	{
		for (i=0; i<imgHeight; i++)
		{
			for (j=0; j < imgWidth; j++)
			{
				output[dim*imgWidth*imgHeight +(i*imgWidth+j)] = source1[dim*imgWidth*imgHeight +(i*imgWidth+j)];
			}
		}	
	}


	//add the 2nd feature map
	for (dim=dim1; dim<dim2; dim++)
	{
		for (i=0; i<imgHeight; i++)
		{
			for (j=0; j < imgWidth; j++)
			{
				output[dim*imgWidth*imgHeight +(i*imgWidth+j)] = source1[dim*imgWidth*imgHeight +(i*imgWidth+j)];
			}
		}	
	}
}
