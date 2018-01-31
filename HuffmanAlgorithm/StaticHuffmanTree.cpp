#include "stdafx.h"
#include "StaticHuffmanTree.h"

StaticHuffmanTree::StaticHuffmanTree() {
	tree = NULL;
	text = "";
	binaryRepresentation = BitVector();
	charFreqTable = std::vector<std::pair<char, unsigned>>();
}
StaticHuffmanTree::StaticHuffmanTree(std::string s) {
	std::vector<std::pair<char, unsigned>> freqTable = make_freq_table(s);	// we make table with the character and their occurances
	charFreqTable = freqTable;
	tree = build_tree(freqTable);	// make a binary tree from it
	text = s;
	codetable ctbl = build_lookup_table(tree);	// assign 0 to every left branch and 1 to right branches
	BitVector t = encode(s, ctbl);	// we transform the input into binary
	binaryRepresentation = t;
}
StaticHuffmanTree::~StaticHuffmanTree() {
	destroy(tree);
}
void StaticHuffmanTree::compressWriteAndPrint(std::ofstream &outf) {
	printInformation();


	std::cout << std::endl;
	outf << charFreqTable.size() << ' ';
	for (std::vector<std::pair<char, unsigned>>::iterator it = charFreqTable.begin(); it != charFreqTable.end(); it++)
	{
		outf << (int)((*it).first) << ' ';
	}
	outf << std::endl;

	for (std::vector<std::pair<char, unsigned>>::iterator it = charFreqTable.begin(); it != charFreqTable.end(); it++)
	{
		outf << (*it).second << ' ';
	}
	outf << std::endl;

	// serialize huffman tree

	for (std::vector<bool>::iterator it = binaryRepresentation.begin(); it != binaryRepresentation.end(); it++)
	{
		outf << *it;
	}
}
void StaticHuffmanTree::decompress(std::ifstream &in, std::ofstream &outf) {
	std::vector<std::pair<char, unsigned>> freqTable = get_freq_table(in);	// get char/freq table from the file
	charFreqTable = freqTable;
	tree = build_tree(charFreqTable);		// we get the tree
	std::string bits;
	std::string line;
	while (!in.eof()) {
		getline(in, line);
		bits += line;
		// ... Append line to buffer and go on
	}
	std::vector<bool> bitVector;
	for (std::string::iterator it = bits.begin(); it != bits.end(); it++) {		// we have the binary representation
		bitVector.push_back(*it - '0');
	}
	in.close();
	std::string text = decode(bitVector, tree);
	this->text = text;
	std::cout << "decoded:\n" << text << std::endl;
	for (std::string::iterator it = text.begin(); it != text.end(); it++)
	{
		outf << *it;
	}
	outf.close();
}
void StaticHuffmanTree::printInformation() {
	std::cout << "original:" << std::endl << text << std::endl;
	std::cout << "encoded (compression ratio: "
		<< (binaryRepresentation.size() + 7) / 8 << "/" << text.size() << " or "
		<< ((float)((binaryRepresentation.size() + 7) / 8) / text.size()) << "):" << std::endl;
	std::cout << "Compressed version is: ";
	std::cout << std::endl;
	Utilities::printBytes(binaryRepresentation);
}
void StaticHuffmanTree::destroy(HuffmanTree* crr) {
	if (crr == NULL) return;
	else {
		destroy(crr->left);
		destroy(crr->right);
		delete crr;
	}
}
void StaticHuffmanTree::print_tree(HuffmanTree* t) {
	{
		std::deque<std::pair<HuffmanTree *, int> > q;
		q.push_back(std::make_pair(t, 0));
		int curlevel = -1;
		while (!q.empty()) {
			HuffmanTree *parent = q.front().first;
			int level = q.front().second;
			q.pop_front();
			if (curlevel != level) {
				curlevel = level;
				std::cout << "Level " << curlevel << std::endl;
			}
			std::cout << parent->cfreq << " " << parent->c << std::endl;
			if (parent->left)
				q.push_back(std::make_pair(parent->left, level + 1));
			if (parent->right)
				q.push_back(std::make_pair(parent->right, level + 1));
		}
	}
}
void StaticHuffmanTree::print_tree() {
	print_tree(tree);
}
