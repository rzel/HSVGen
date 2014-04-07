#include <stdlib.h>
#include <math.h>
#include <opencv2/opencv.hpp>

struct Node
{
	int id;
	Node *parent;
	Node *leftChild;
	Node *rightChild;
	double L;
	double ab;
	double size;
	double shape;
	int isRoot;
};

struct uchar4
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};

struct cie_lab
{
	double a;
	double b;
	double l;
};

// Standard conversion of rgb to Lab via following route rgb to XYZ to Lab
struct cie_lab* rgbtolab(int R, 
						 int G, 
						 int B)
{
	double var_R = ( R / 255.0 )  ;      //R from 0 to 255
	double var_G = ( G / 255.0 )  ;      //G from 0 to 255
	double var_B = ( B / 255.0 )  ;      //B from 0 to 255

	if (var_R > 0.04045)
		var_R = pow(( ( var_R + 0.055) / 1.055), 2.4);
	else                   
		var_R = var_R / 12.92;

	if (var_G > 0.04045)
		var_G = pow(((var_G + 0.055) / 1.055), 2.4);
	else                  
		var_G = var_G / 12.92;
	
	if (var_B > 0.04045) 
		var_B = pow(((var_B + 0.055) / 1.055) ,2.4);
	else                   
		var_B = var_B / 12.92;

	var_R = var_R * 100;
	var_G = var_G * 100;
	var_B = var_B * 100;

	//Observer. = 2°, Illuminant = D65
	double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
	double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
	double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

	double ref_X = 95.047;
	double ref_Y = 100.000;
	double ref_Z = 108.883;

	double var_X = X / ref_X  ;        //ref_X =  95.047   Observer= 2°, Illuminant= D65
	double var_Y = Y / ref_Y  ;        //ref_Y = 100.000
	double var_Z = Z / ref_Z  ;        //ref_Z = 108.883

	if (var_X > 0.008856) 
		var_X = pow(var_X , (1.0/3));
	else                    
		var_X = (7.787 * var_X) + (16.0 / 116);
	
	if (var_Y > 0.008856) 
		var_Y = pow(var_Y , (1.0/3));
	else                    
		var_Y = (7.787 * var_Y) + (16.0 / 116);
	
	if (var_Z > 0.008856) 
		var_Z = pow(var_Z , (1.0/3));
	else                    
		var_Z = (7.787 * var_Z) + (16.0 / 116);

	double l = (116 * var_Y) - 16;
	double a = 500 * ( var_X - var_Y );
	double b = 200 * ( var_Y - var_Z );

	struct cie_lab *lab = (struct cie_lab*)malloc(sizeof(struct cie_lab));

	lab->a = a;
	lab->b = b;
	lab->l = l;

	return lab;
}

void preprocess(char *inputFilename, struct Node *node, int *numPixels, int *imageRows, int *imageCols)
{

	// Load image
	cv::Mat image = cv::imread(inputFilename, CV_LOAD_IMAGE_COLOR);
	cv::Mat imageRGBA;
  	cv::cvtColor(image, imageRGBA, CV_BGR2RGBA);

  	// Initialize values
	*imageRows = image.rows;
	*imageCols = image.cols;
	*numPixels = *imageRows * *imageCols;

	// Allocate Nodes/leaves of the binary tree 
	node = (struct Node*)malloc(*numPixels * sizeof(struct Node));

	unsigned char *cvPtr = imageRGBA.ptr<unsigned char>(0);

	struct cie_lab *tempLab = (struct cie_lab*)malloc(sizeof(struct cie_lab));

	// Initialize values of node attributes
	for(int i = 0; i < *numPixels; ++i)
	{
		tempLab = rgbtolab((int)cvPtr[4*i], (int)cvPtr[4*i + 1], (int)cvPtr[4*i + 2]);

		node[i].L = tempLab->l;
		node[i].ab = tempLab->a * tempLab->b;
		node[i].size = 1;
		node[i].id = i;
		node[i].leftChild = NULL;
		node[i].rightChild = NULL;
		node[i].parent = NULL;
		node[i].isRoot = 1;
		// Shape value to be added
	}
}