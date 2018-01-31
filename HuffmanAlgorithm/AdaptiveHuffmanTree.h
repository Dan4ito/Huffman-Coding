#pragma once
#include "Utilities.h"
#include <fstream>
#include <string>

using namespace std;
// A Huffman Tree Node
struct AdaptiveHuffmanTreeNode {
	char c; // character in an alphabet
	int cfreq; // frequency of c.
	AdaptiveHuffmanTreeNode *left;
	AdaptiveHuffmanTreeNode *right;
	int order;
	bool NYT;		// not yet transmitted
	struct AdaptiveHuffmanTreeNode *parent;

	AdaptiveHuffmanTreeNode(char c, int cfreq, AdaptiveHuffmanTreeNode *left = NULL,
		struct AdaptiveHuffmanTreeNode *right = NULL, int order = 512, bool NYT = false, AdaptiveHuffmanTreeNode *parent = NULL) :
		c(c), cfreq(cfreq), left(left), right(right), order(order), NYT(NYT), parent(parent) {}

};

static AdaptiveHuffmanTreeNode*& copy(const AdaptiveHuffmanTreeNode* other);

class AdaptiveHuffmanTree {
private:
	AdaptiveHuffmanTreeNode * root;

	void destroy(AdaptiveHuffmanTreeNode* crr);
	void incrementParents(AdaptiveHuffmanTreeNode* node);
	AdaptiveHuffmanTreeNode* search_helper(AdaptiveHuffmanTreeNode* root, const char& symbol, string& path);
	string getPath_helper(AdaptiveHuffmanTreeNode* root, const char& symbol, string path);
	AdaptiveHuffmanTreeNode* findNYT_helper(AdaptiveHuffmanTreeNode* root, string& path);
public:
	AdaptiveHuffmanTree();
	~AdaptiveHuffmanTree();
	void addAllWithSameWeight(int weight, vector<AdaptiveHuffmanTreeNode*>& nodes, AdaptiveHuffmanTreeNode* node, AdaptiveHuffmanTreeNode* root);
	void searchSameWeightNodesAndSwap(AdaptiveHuffmanTreeNode*& node);
	void growTree(const char& symbol);
	void decode(string inFile, string outFile);
	void encode(const char& symbol, ofstream& out, string& binaryRepresentation);
	AdaptiveHuffmanTreeNode* search(const char& symbol, string& path);
	string getPath(const char& symbol, string& path);
	void updateTree(AdaptiveHuffmanTreeNode* nodeToBeUpdated);
	AdaptiveHuffmanTreeNode* findNYT(string& path);
};