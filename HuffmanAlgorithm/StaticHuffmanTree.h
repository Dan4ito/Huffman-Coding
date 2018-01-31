#pragma once

#include<map>
#include<string>
#include<fstream>
#include<queue>
#include "Utilities.h"

typedef std::vector<bool> BitVector;
typedef std::map<char, BitVector> codetable;

// A Huffman Tree Node
struct HuffmanTree {
	char c; // character in an alphabet
	int cfreq; // frequency of c.
	HuffmanTree *left;
	HuffmanTree *right;
	HuffmanTree(char c, int cfreq, HuffmanTree *left = NULL,
		HuffmanTree *right = NULL) :
		c(c), cfreq(cfreq), left(left), right(right) {
	}
	// Compare two tree nodes
	class Compare {
	public:
		bool operator()(HuffmanTree *a, HuffmanTree *b) {
			return a->cfreq > b->cfreq;
		}
	};
};
class StaticHuffmanTree {
private:
	HuffmanTree * tree;
	std::string text;
	BitVector binaryRepresentation;
	std::vector<std::pair<char, unsigned>> charFreqTable;
	void destroy(HuffmanTree* crr);
	// Prints the tree nodes in breadth-first order
	void print_tree(HuffmanTree *t);
public:
	StaticHuffmanTree();
	StaticHuffmanTree(std::string s);
	~StaticHuffmanTree();
	void compressWriteAndPrint(std::ofstream &outf);
	void decompress(std::ifstream &in, std::ofstream &outf);
	void printInformation();
	void print_tree();
	static std::vector<std::pair<char, unsigned> > make_freq_table(std::string inp) {
		std::map<char, unsigned> cfmap;
		std::vector<std::pair<char, unsigned> >cfvec;
		for (unsigned i = 0; i < inp.size(); i++) {		// we fill the map
			if (cfmap.find(inp[i]) == cfmap.end()) {
				cfmap.insert(std::make_pair(inp[i], 0));
			}
			cfmap[inp[i]] += 1;
		}
		for (std::map<char, unsigned>::iterator it = cfmap.begin();		// we fill the vector
			it != cfmap.end(); ++it) {
			cfvec.push_back(std::make_pair(it->first, it->second));
		}
		return cfvec;
	}
	static HuffmanTree * build_tree(std::vector<std::pair<char, unsigned> > &alph) {
		std::priority_queue<HuffmanTree *, std::vector<HuffmanTree *>, HuffmanTree::Compare > alph_heap;		// Step 1: we sort the array according to occurances (that is how the algorithm works! )
		for (std::vector<std::pair<char, unsigned> >::iterator it = alph.begin();
			it != alph.end(); ++it) {
			HuffmanTree *leaf = new HuffmanTree(it->first, it->second);
			alph_heap.push(leaf);
		}
		// HuffmanTree algorithm: Merge two lowest weight leaf nodes until
		// only one node is left (root).
		HuffmanTree *root = NULL;		// Step 2: From the freq table, select two with least frequences (thats why use a MIN heap) and create nodes from the  (that is how the algorithm works! )
		while (alph_heap.size() > 1) {
			HuffmanTree *l, *r;
			l = alph_heap.top();
			alph_heap.pop();
			r = alph_heap.top();		// note that the array must be always sorted so the MIN heap is Perfect!
			alph_heap.pop();
			root = new HuffmanTree(0, l->cfreq + r->cfreq, l, r);		// the new node is with frequence (l.freq + r.freq) and we insert it in the heap (that is how the algorithm works!)
			alph_heap.push(root);		// repeat until only 1 symbol left
		}
		return root;		// binary tree is complete!
	}
	// Makes a lookup table (std::map) of (c -> code) from a HuffmanTree, where
	// code is an unsigned long representing the binary code.
	static std::map<char, BitVector> build_lookup_table(HuffmanTree *htree) {
		codetable lookup;		// map --- Character -> Binary Representation for it
		std::deque<std::pair<HuffmanTree *, BitVector> > q;
		q.push_back(make_pair(htree, BitVector()));
		while (!q.empty()) {		// Basically a BFS
			HuffmanTree *node, *lc, *rc;
			BitVector code;
			node = q.front().first;
			code = q.front().second;
			q.pop_front();
			lc = node->left;
			rc = node->right;
			if (lc) {
				// HuffmanTree is always full (either no children or two children)
				// Left child is appended a 0 and right child a 1.
				BitVector code_cp(code);
				q.push_back(make_pair(lc, (code.push_back(0), code)));
				q.push_back(make_pair(rc, (code_cp.push_back(1), code_cp)));
			}
			else {
				// Leaf node: contains the character, add to map
				lookup.insert(make_pair(node->c, code));
			}
		}
		return lookup;		// we return the map of characters and binary representation
	}
	// Encodes an input string. returns a byte vector.
	static BitVector encode(std::string input, codetable &lookup) {
		BitVector result;
		for (std::string::iterator it = input.begin(); it != input.end(); ++it) {
			BitVector b = lookup[*it];
			result.insert(result.end(), b.begin(), b.end());
		}
		return result;
	}
	static std::vector<std::pair<char, unsigned>> get_freq_table(std::ifstream &in) {
		std::string chars;
		std::getline(in, chars);
		std::vector<char> treeChars;
		for (auto elem : chars) {
			treeChars.push_back(elem);
		}
		char letter;
		in >> letter;
		std::string frequency;
		std::getline(in, frequency);
		frequency = letter + frequency;
		std::vector<std::string> stringTreeFrequency = Utilities::split(frequency);
		stringTreeFrequency.pop_back();
		std::vector<unsigned> treeFreq;
		for (std::vector<std::string> ::iterator it = stringTreeFrequency.begin(); it != stringTreeFrequency.end(); it++) {
			treeFreq.push_back(stoi(*it));
		}
		std::vector<std::pair<char, unsigned>> charFreqVector;
		std::vector<unsigned> ::iterator it1;
		std::vector<char> ::iterator it2;
		for (it1 = treeFreq.begin(), it2 = treeChars.begin(); (it1 != treeFreq.end() && it2 != treeChars.end()); it1++, it2++) {
			charFreqVector.push_back(std::make_pair(*it2, *it1));
		}
		return charFreqVector;
	}
	// Look up the next valid code in @biter using @htree and returns the
	// resulting string. Note the iterator @biter is advanced by the actual
	// length of the next valid code, which varies.
	static char code_lookup(BitVector::iterator &biter, const BitVector::iterator &biter_end,
		const HuffmanTree *htree) {
		const HuffmanTree *node = htree;
		while (true) {	// we have a vector from bits '1010110100'
			if (!node->left) {
				// Huffman tree is full: always contains both children or none. We have found a charater in the tree
				break;
			}
			if (biter == biter_end) {
				throw std::out_of_range("No more bits");
			}
			if (*biter) {		// if the bit is 1
				node = node->right;
			}
			else {				// if the bit is 0
				node = node->left;
			}
			++biter;
		}
		return node->c;
	}
	// Decodes a compressed string represented by a bit vector (vector<char>)
	// @compressed, using a HuffmanTree @htree.
	// Returns the original string.
	static std::string decode(BitVector &compressed, const HuffmanTree *htree) {
		std::string result;
		BitVector::iterator biter = compressed.begin();
		while (true) {
			try {
				result += code_lookup(biter, compressed.end(), htree);		// here we gather the actual text
			}
			catch (const std::out_of_range &oor) {		// if we've reached .end()
														// Iterator exhausted.
				break;
			}
		}
		return result;
	}
};
