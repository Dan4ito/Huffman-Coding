#include "stdafx.h"
#include "AdaptiveHuffmanTree.h"

static AdaptiveHuffmanTreeNode*& copy(const AdaptiveHuffmanTreeNode* other) {
	AdaptiveHuffmanTreeNode* newNode;
	if (other == NULL) {
		newNode = NULL;
		return newNode;
	}
	else {
		newNode = new AdaptiveHuffmanTreeNode(other->c, other->cfreq, copy(other->left), copy(other->right), other->order, other->NYT, other->parent);
		return newNode;
	}
};

void AdaptiveHuffmanTree::destroy(AdaptiveHuffmanTreeNode* crr) {
	if (crr == NULL) return;
	else {
		destroy(crr->left);
		destroy(crr->right);
		delete crr;
	}
}
void AdaptiveHuffmanTree::incrementParents(AdaptiveHuffmanTreeNode* node) {	// if the rule : a node with a higher freq must have higher node number is violated then we do this
	while (node != NULL) {
		searchSameWeightNodesAndSwap(node);
		node->cfreq++;
		node = node->parent;
	}
}
AdaptiveHuffmanTreeNode* AdaptiveHuffmanTree::search_helper(AdaptiveHuffmanTreeNode* root, const char& symbol, string& path) {
	if (root != NULL) {
		if (root->c == symbol) {
			return root;
		}
		else {
			path.push_back('0');
			AdaptiveHuffmanTreeNode* foundNode = search_helper(root->left, symbol, path);
			if (foundNode == NULL) {
				path.push_back('1');
				foundNode = search_helper(root->right, symbol, path);
			}
			{if (foundNode == NULL && path.length() > 0) path.pop_back();
			return foundNode; }
		}
	}
	else {
		path.pop_back(); return NULL;
	}
}
string AdaptiveHuffmanTree::getPath_helper(AdaptiveHuffmanTreeNode* root, const char& symbol, string path) {
	if (root == NULL) {
		return "Not Here";
	}
	if (root->c == symbol) return path;

	else
	{
		path = getPath_helper(root->left, symbol, path + '0');

		if (path == "Not Here") {
			path = "";
			path = getPath_helper(root->right, symbol, path + '1');
		}
	}
	return path;
}
AdaptiveHuffmanTreeNode* AdaptiveHuffmanTree::findNYT_helper(AdaptiveHuffmanTreeNode* root, string& path) {
	if (root != NULL) {
		if (root->NYT == true) {
			return root;
		}
		else {
			path.push_back('0');
			AdaptiveHuffmanTreeNode* foundNode = findNYT_helper(root->left, path);
			if (foundNode == NULL) {
				path.push_back('1');
				foundNode = findNYT_helper(root->right, path);
			}
			if (foundNode == NULL && path.length() > 0) path.pop_back();
			return foundNode;
		}
	}
	else {
		path.pop_back();
		return NULL;	// althought there is always NYT Node
	}
}
AdaptiveHuffmanTree::AdaptiveHuffmanTree() {
	root = new AdaptiveHuffmanTreeNode(0, 0, NULL, NULL, 512, true, NULL);
}
AdaptiveHuffmanTree::~AdaptiveHuffmanTree() {
	destroy(root);
}

void AdaptiveHuffmanTree::addAllWithSameWeight(int weight, vector<AdaptiveHuffmanTreeNode*>& nodes, AdaptiveHuffmanTreeNode* node, AdaptiveHuffmanTreeNode* root) {
	if (root == NULL) return;
	if (root->cfreq == weight && root != node) nodes.push_back(root);
	addAllWithSameWeight(weight, nodes, node, root->left);
	addAllWithSameWeight(weight, nodes, node, root->right);

}
void AdaptiveHuffmanTree::searchSameWeightNodesAndSwap(AdaptiveHuffmanTreeNode*& node) {
	if (node->parent == NULL) return;
	vector<AdaptiveHuffmanTreeNode*> sameWeightNodes;
	addAllWithSameWeight(node->cfreq, sameWeightNodes, node, this->root->left);
	addAllWithSameWeight(node->cfreq, sameWeightNodes, node, this->root->right);
	if (sameWeightNodes.size() == 0) return;
	AdaptiveHuffmanTreeNode* nodeToSwap = sameWeightNodes[0];
	for (size_t i = 1; i < sameWeightNodes.size(); i++)
	{
		if (sameWeightNodes[i]->order > nodeToSwap->order) {
			nodeToSwap = sameWeightNodes[i];
		}
	}
	if (node->order < nodeToSwap->order) {

		AdaptiveHuffmanTreeNode *&firstRef = node->parent->left == node ? node->parent->left : node->parent->right;
		AdaptiveHuffmanTreeNode *&secondRef = nodeToSwap->parent->left == nodeToSwap ? nodeToSwap->parent->left : nodeToSwap->parent->right;

		std::swap(firstRef, secondRef);
		std::swap(firstRef->parent, secondRef->parent);
		std::swap(firstRef->order, secondRef->order);


	}
}
void AdaptiveHuffmanTree::growTree(const char& symbol) {
	string path;
	AdaptiveHuffmanTreeNode* foundNode = search(symbol, path);
	if (foundNode != NULL) {	//  there is already a node with that character
		updateTree(foundNode);
	}
	else {
		AdaptiveHuffmanTreeNode* NYTnode = findNYT(path);
		NYTnode->NYT = false;
		NYTnode->right = new AdaptiveHuffmanTreeNode(symbol, 0, NULL, NULL, NYTnode->order - 1, false, NYTnode);
		NYTnode->left = new AdaptiveHuffmanTreeNode(0, 0, NULL, NULL, NYTnode->order - 2, true, NYTnode);
		updateTree(NYTnode->right);


	}
}
void AdaptiveHuffmanTree::decode(string inFile, string outFile) {

	AdaptiveHuffmanTreeNode* currentNode = root;

	ifstream in;
	in.open(inFile);
	if (!in)
	{
		// Print an error and exit
		cerr << "Opening error!" << endl;
		return;
	}

	ofstream out;
	out.open(outFile);

	if (!out)
	{
		// Print an error and exit
		cerr << "Opening error!" << endl;
		return;
	}

	char symbolToRead;
	char symbolFromBits;
	char actualLetter;
	string text;
	while (!in.eof()) {
		if (currentNode->left == NULL && currentNode->right == NULL) {		// is leaf
			if (currentNode->NYT == true) {		// is NYT
				string eightBits;
				in >> symbolToRead;
				eightBits.push_back(symbolToRead);
				for (size_t i = 0; i < 7; i++)		// read the next 8 bits
				{
					in.get(symbolToRead);
					eightBits.push_back(symbolToRead);
				}
				actualLetter = Utilities::asciiToChar(eightBits);		// transform to char
			}
			else {
				actualLetter = currentNode->c;
			}
			text += actualLetter;
			growTree(actualLetter);
			currentNode = root;
			out << actualLetter;
		}
		else {
			in >> symbolToRead;
			if (symbolToRead == '0') { currentNode = currentNode->left; }
			else { currentNode = currentNode->right; }
		}
	}
	in.close();
	cout << "original:" << endl;
	cout << text;

	inFile = "";
	outFile = "";
	cout << endl;



}
void AdaptiveHuffmanTree::encode(const char& symbol, ofstream& out, string& binaryRepresentation) {
	string path;
	AdaptiveHuffmanTreeNode* foundNode = search(symbol, path);
	if (foundNode != NULL) {	//  if already in the tree just update the nodes
		out << path;
		binaryRepresentation += path;
		updateTree(foundNode);
	}
	else {		// if the character isn't encountered before, NYT node spawns two nodes, the right node is the character, the left the new NYT, then update weights
		AdaptiveHuffmanTreeNode* NYTnode = findNYT(path);
		out << path;
		binaryRepresentation += path;
		out << Utilities::getAscii(symbol);
		binaryRepresentation += Utilities::getAscii(symbol);
		NYTnode->NYT = false;
		NYTnode->right = new AdaptiveHuffmanTreeNode(symbol, 0, NULL, NULL, NYTnode->order - 1, false, NYTnode);
		NYTnode->left = new AdaptiveHuffmanTreeNode(0, 0, NULL, NULL, NYTnode->order - 2, true, NYTnode);
		updateTree(NYTnode->right);
	}
}
AdaptiveHuffmanTreeNode* AdaptiveHuffmanTree::search(const char& symbol, string& path) {
	return search_helper(this->root, symbol, path);
}
string AdaptiveHuffmanTree::getPath(const char& symbol, string& path) {
	return getPath_helper(root, symbol, path);
}
void AdaptiveHuffmanTree::updateTree(AdaptiveHuffmanTreeNode* nodeToBeUpdated) {
	nodeToBeUpdated->cfreq++;
	incrementParents(nodeToBeUpdated->parent);
}
AdaptiveHuffmanTreeNode* AdaptiveHuffmanTree::findNYT(string& path) {
	return findNYT_helper(root, path);
}