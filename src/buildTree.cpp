#include <stdlib.h>
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <float.h>
#include <vector>
#include <algorithm>

struct Node
{
	long int id;
	Node *parent;
	Node *leftChild;
	Node *rightChild; 
	double L;
	double ab;
	double size;
	double shape;
	int isRoot;
};

struct Entry
{
	long int first;
	long int second;
	double costInv;
};

using namespace std;

struct Heap
{
	long int length;
	long int heapSize;
	vector<Entry> array;	
};

long int parentof(long int i)
{
	return (i - 1) / 2;
}

long int left_child(long int i)
{
	return 2 * i + 1;
}

long int right_child(long int i)
{
	return 2 * (i + 1);
}

void max_heapify(struct Heap* a, long int i)
{
	long int l, r, largest;

	l = left_child(i);
	r = right_child(i);
	
	if(l < a->heapSize && a->array[l].costInv > a->array[i].costInv)
	{
		largest = l;
	}
	else largest = i;

	if(r < a->heapSize && a->array[r].costInv > a->array[largest].costInv)
	{
		largest = r;
	}
	if(largest != i)
	{
		double temp = a->array[i].costInv;
		long int tempFirst = a->array[i].first;
		long int tempSecond = a->array[i].second;
		a->array[i].costInv = a->array[largest].costInv;
		a->array[i].first = a->array[largest].first;
		a->array[i].second = a->array[largest].second;
		a->array[largest].costInv = temp; 
		a->array[largest].first = tempFirst;
		a->array[largest].second = tempSecond;
		max_heapify(a, largest);
	}
	
}


void build_max_heap(struct Heap* a)
{
	a->heapSize = a->length;

	for(long int i = (a->length/2) - 1; i >= 0; --i)
	{
		max_heapify(a, i);
	}
}

double heap_maximum(struct Heap* a)
{
	return a->array[0].costInv;
}

Entry* heap_extract_max(struct Heap* a)
{
	if(a->heapSize < 1)
	{
		printf("heap underflow\n");
		return NULL;
	}
	Entry *max = new Entry[1];
	max->first = a->array[0].first;
	max->second = a->array[0].second;
	max->costInv = a->array[0].costInv;
	a->array[0] = a->array[a->heapSize - 1];          // COPY OCCURS OR NOT
	a->array.erase(a->array.begin() + a->heapSize - 1);
	a->heapSize--;
    max_heapify(a, 0);

	return max;
}

void heap_increase_key(struct Heap* a, long int i, double key)
{
	if(key < a->array[i].costInv)
	{
		printf("new key is smaller than current key");
		return;
	}
	a->array[i].costInv = key;
	while(i > 1 && a->array[parentof(i)].costInv < a->array[i].costInv)
	{
		double temp = a->array[i].costInv;
		long int tempFirst = a->array[i].first;
		long int tempSecond = a->array[i].second;
		a->array[i].costInv = a->array[parentof(i)].costInv;
		a->array[i].first = a->array[parentof(i)].first;
		a->array[i].second = a->array[parentof(i)].second;
		a->array[parentof(i)].costInv = temp;
		a->array[parentof(i)].first = tempFirst;
		a->array[parentof(i)].second = tempSecond;

		i = parentof(i);
	}
}

void max_heap_insert(struct Heap* a, double key, long int f, long int s)
{
	a->heapSize += 1;
	Entry* temp = (Entry *) malloc(sizeof(Entry));
	temp->costInv = -DBL_MAX;  // -ve of infinity
	temp->first = f;
	temp->second = s;
	a->array.push_back(temp[0]);
	heap_increase_key(a, a->heapSize - 1, key);
}


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


	long int segmentsNOW = numPixels;
	long int counter = numPixels;
	vector<long int> myVector;

	while(segmentsNOW > numSegments)
	{
		int check = 0;
		long int first;
		long int second;
		double minCost;
		for(long int i = 0; i < counter; ++i)
		{
			for(long int j = i + 1; j < counter; ++j)
			{
				if(myVector.empty() || !((find(myVector.begin(), myVector.end(), i) != myVector.end()) || (find(myVector.begin(), myVector.end(), j) != myVector.end())))
				{
					if(check == 0)
					{
						first = i;
						second = j;
						minCost = computeCostInv(&node[i], &node[j]);
						check = 1;
					}
					else
					{
						if(minCost > computeCostInv(&node[i], &node[j]))
						{
							first = i;
							second = j;
							minCost = computeCostInv(&node[i], &node[j]);
						}
					}
				}
			}
			printf("%ld --- %ld\n", first, second);
		}

		printf("%ld --- %ld\n", first, second);

		myVector.push_back(first);
		myVector.push_back(second);

		node = (Node*) realloc(node, (counter + 1) * sizeof(Node));

		node[counter].L = (node[first].L + node[second].L) * 0.5;
		node[counter].ab = (node[first].ab + node[second].ab) * 0.5;
		node[counter].size = node[first].size + node[second].size;
		node[counter].id = counter;
		node[counter].leftChild = &node[first];
		node[counter].rightChild = &node[second];
		node[counter].parent = NULL;
		node[first].parent = &node[counter];
		node[second].parent = &node[counter];
		node[first].isRoot = 0;
		node[second].isRoot = 0;
		node[counter].isRoot = 1;
		node[counter].shape = 0;

		counter++;
		segmentsNOW--;
	}


	for(long int i = 0; i < counter; ++i)
	{
		if(node[i].isRoot == 1)
		{
			printf("%ld\n", node[i].id);
		}
	}

	/* Blueprint:
		Initialize the leaves of the tree as the pixels of the image i.e. nodes passed in the argument.
		Initialize an adptable max priority queue with the key as the pairs of the leaves/pixels and value as the Cost inverse of the pair of nodes calculated using compCostInv
		Now extract max from the priority queue => check the key and make the corresponding pixels or leaves or nodes as the children of the union
		Say x1 U x2 is the key of the max entry => replace all priority queue entries with x1 as one of the entry of the key with x1 U x2 as the entry and hence change the attributes of the entry
		Also remove all those entries with x2 as one of the key of the entry and hence correct the configuration of the max priority queue
		Now repeat again form step 3 until  we get the desired number of segments
	*/
	/* 
	We should return the roots/Nodes at top level of the tree 
	Hence, that has to be rectified in the code
	*/


}