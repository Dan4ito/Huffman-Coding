// HuffmanAlgorithm.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "StaticHuffmanTree.h"
#include "AdaptiveHuffmanTree.h"
using namespace std;
string inFile;		// global inputFile
string outFile;		// global outputFile
void getTheCommand(std::string command);
int main()
{
	Utilities::printMenu();
	string command;
	bool loop = true;
	while (loop) {
		cout << "Enter a command: ";
		getline(cin, command);
		if (command == "-exit") break;
		getTheCommand(command);
	}
}
void getTheCommand(std::string command)
{
	vector<string> parts = Utilities::split(command);
	if (parts[0] == "-i") {
		if (parts.size() != 2) { std::cout << "Invalid command!" << '\n'; return; }
		inFile = parts[1];
	}
	else if (parts[0] == "-o") {
		if (parts.size() != 2) { std::cout << "Invalid command!" << '\n'; return; }
		outFile = parts[1];
	}
	else if (parts[0] == "-c") {
		if (parts.size() != 1) { cout << "Invalid command!" << '\n'; return; }
		if (inFile == "") { cout << "No file to load from!" << '\n'; return; }
		if (outFile == "") { cout << "No file to output to!" << '\n'; return; }
		ifstream in;
		in.open(inFile);
		if (!in)
		{
			// Print an error and exit
			cerr << "Opening error!" << endl;
			return;
		}
		string s;
		string line;
		while (!in.eof()) {
			getline(in, line);
			s += line;
			// ... Append line to buffer and go on
		}
		in.close();
		StaticHuffmanTree* htree = new StaticHuffmanTree(s);
		ofstream outf(outFile);
		// If we couldn't open the output file stream for writing
		if (!outf)
		{
			// Print an error and exit
			cerr << "Opening error!" << endl;
			return;
		}
		htree->compressWriteAndPrint(outf);
		delete htree;
		inFile = "";
		outFile = "";
		cout << endl;
	}
	else if (parts[0] == "-d") {
		if (parts.size() != 1) { cout << "Invalid command!" << '\n'; return; }
		if (outFile == "") { cout << "No file to output to!" << '\n'; return; }
		if (inFile == "") { cout << "No file to load from!" << '\n'; return; }
		ifstream in(inFile);
		if (!in)
		{
			// Print an error and exit
			cerr << "Opening error!" << endl;
			return;
		}
		StaticHuffmanTree* decompressed = new StaticHuffmanTree();
		ofstream outf(outFile);
		decompressed->decompress(in, outf);
		delete decompressed;
		inFile = "";
		outFile = "";
	}
	else if (parts[0] == "-cAdaptive") {
		if (parts.size() != 1) { cout << "Invalid command!" << '\n'; return; }
		if (inFile == "") { cout << "No file to load from!" << '\n'; return; }
		if (outFile == "") { cout << "No file to output to!" << '\n'; return; }
		ifstream in;
		in.open(inFile);
		if (!in)
		{
			// Print an error and exit
			cerr << "Opening error!" << endl;
			return;
		}
		ofstream outf(outFile);
		// If we couldn't open the output file stream for writing
		if (!outf)
		{
			// Print an error and exit
			cerr << "Opening error!" << endl;
			return;
		}
		AdaptiveHuffmanTree* adaptiveTree = new AdaptiveHuffmanTree();
		char symbol;
		string binaryRepresentation;
		string text;
		while (in.get(symbol)) {
			text += symbol;
			adaptiveTree->encode(symbol, outf, binaryRepresentation);
		}
		in.close();
		cout << "original:" << endl << text << endl;
		cout << "encoded (compression ratio: "
			<< (binaryRepresentation.size() + 7) / 8 << "/" << text.size() << " or "
			<< ((float)((binaryRepresentation.size() + 7) / 8) / text.size()) << "):" << endl;
		cout << "Compressed version is: ";
		vector<bool> bits;
		for (string::iterator it = binaryRepresentation.begin(); it != binaryRepresentation.end(); it++) {
			if (*it == '0')
				bits.push_back(false);
			else bits.push_back(true);
		}
		Utilities::printBytes(bits);
		delete adaptiveTree;
		inFile = "";
		outFile = "";
		cout << endl;
	}
	else if (parts[0] == "-dAdaptive") {
		AdaptiveHuffmanTree* adaptiveTree = new AdaptiveHuffmanTree();
		adaptiveTree->decode(inFile, outFile);
		delete adaptiveTree;
	}
	else {
		std::cout << "Invalid command!" << '\n';
		return;
	}
}
