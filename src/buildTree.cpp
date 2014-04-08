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
	/* Blueprint:
		Initialize the leaves of the tree as the pixels of the image i.e. nodes passed in the argument.
		Initialize an adptable max priority queue with the key as the pairs of the leaves/pixels and value as the Cost inverse of the pair of nodes calvulated using compCostInv
		Now extract max from the priority queue => check the key and make the corresponding pixels or leaves or nodes as the children of the union
		Say x1 U x2 is the key of the max entry => replace all priority queue entries with x1 as one of the entry of the key with x1 U x2 as the entry and hence change the attributes of the entry
		Also remove all those entries with x2 as one of the key of the entry and hence correct the configuration of the max priority queue
		Now repeat again form step 3 until  we get the desired number of segments
	*/
	/* 
	We should return the roots/Nodes at top level of the tree 
	// Hence, that has to be rectified in the code

}