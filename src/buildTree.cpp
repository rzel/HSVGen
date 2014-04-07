#include <stdlib.h>
#include <math.h>

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

double computeCostInv(struct Node* node1, 
					  struct Node* node2)
{
	double omegaSize = exp(1.5);
	double q_size, q_shape, q_color, q_luminance;
	double resultSize = node1->size + node2->size;
	double resultLuminance = (node1->L + node2->L) * 0.5;
	double resultAB = (node1->ab + node2->ab) * 0.5;
	
	q_size = pow(node1->size / (resultSize), omegaSize - 1) * pow(node2->size / (resultSize), omegaSize - 1);
	q_luminance = exp((pow(node1->L - resultLuminance, 2) * node1->size) / resultSize) * exp((pow(node2->L - resultLuminance, 2) * node2->size) / resultSize);
	q_color = exp((pow(node1->ab - resultAB, 2) * node1->size) / resultSize) * exp((pow(node2->ab - resultAB, 2) * node2->size) / resultSize);
	q_shape = 1; // to be set

	return q_size * q_shape * q_color * q_luminance;
}

void buildTree(struct Node* node, 
			   int numPixels,
			   int numSegments)
{
	// Need to be implemented
}