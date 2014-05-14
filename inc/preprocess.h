#ifndef PREPROCESS_H__
#define PREPROCESS_H__

struct Node;

struct cie_lab 
{
	double a;
	double b;
	double l;
};

struct cie_lab* rgbtolab(int R,
						 int G, 
						 int B);

void preprocess(char *inputFilename,
			    struct Node **node,
			    long int *numPixels,
			    int *imageRows,
			    int *imageCols);

#endif 