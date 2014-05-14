#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <opencv2/opencv.hpp> 

using namespace std;

struct uchar4
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};




void preprocess(char *inputFilename, int *numPixels, int *imageRows, int *imageCols)
{

	// Load image
	cv::Mat image = cv::imread(inputFilename, CV_LOAD_IMAGE_COLOR);
	cv::Mat imageRGBA;
  	cv::cvtColor(image, imageRGBA, CV_BGR2RGBA);

  	// Initialize values
	*imageRows = image.rows;
	*imageCols = image.cols;
	*numPixels = *imageRows * *imageCols;

	unsigned char *cvPtr = imageRGBA.ptr<unsigned char>(0);

	cv::Mat M(image.rows, image.cols, CV_8UC3, cv::Scalar(255,255,255));

	// Initialize values of node attributes
	for(int i = 0; i < image.rows; ++i)
	{
		for(int j = 0; j < image.cols - 1; ++j)
		{
			if((int)cvPtr[4*(image.cols * i + j)] != (int)cvPtr[4*(image.cols * i + j + 1)]  && (int)cvPtr[4*(image.cols * i + j) + 1] != (int)cvPtr[4*(image.cols * i + j + 1) + 1]  && (int)cvPtr[4*(image.cols * i + j) + 2] != (int)cvPtr[4*(image.cols * i + j + 1) + 2])
				cout << i << "  " << j << endl;
		}
		//printf("%d %d %d\n",(int)cvPtr[4*i], (int)cvPtr[4*i + 1], (int)cvPtr[4*i + 2]);
	}
	
}


int main(int argc, char **argv)
{
	char *input_file;
	char *output_file;
	int numSegments;
	int numPix;
	int imageRows;
	int imageCols;
	input_file = argv[1];

	preprocess(input_file, &numPix, &imageRows, &imageCols);
	
	return 0;

}