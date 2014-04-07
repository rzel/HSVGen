#ifndef BUILDTREE_H__
#define BUILDTREE_H__ 

struct Node;

double computeCostInv(struct Node* node1,
					  struct Node* node2);

void buildTree(struct Node* node,
			   int numPixels,
			   int numSegments);

#endif