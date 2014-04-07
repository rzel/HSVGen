#include <stdlib.h> 
#include <stdio.h>
#include "../inc/buildTree.h"
#include "../inc/preprocess.h"

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


void preprocess(char *inputFilename,
			    struct Node *node,
			    int *numPixels,
			    int *imageRows,
			    int *imageCols);


void buildTree(struct Node* node,
			   int numPixels,
			   int numSegments);



int main(int argc, char **argv)
{
	char *input_file;
	char *output_file;
    char *default_output_file = "output.png";
	int numSegments;
	int numPix;
	int imageRows;
	int imageCols;
	struct Node* node;


	switch (argc)
	{
	case 3:
		input_file = argv[1];
		numSegments = atoi(argv[2]);
		output_file = default_output_file;
		break;
	case 4:
		input_file  = argv[1];
		numSegments = atoi(argv[2]);
		output_file = argv[3];
		break;
	default:
		printf("Usage: ./hsvgen input_filename number_of_segments [output_filename]");
		exit(1);
	}

	preprocess(input_file, node, &numPix, &imageRows, &imageCols);

	buildTree(node, numPix, numSegments);
	
	return 0;

}