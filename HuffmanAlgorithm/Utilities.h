#pragma once
#include <vector>
#include <iostream>
#include<iomanip>
class Utilities
{
public:
	static void printByte(std::vector<bool> byte) {
		int sum = 0;
		int counter = 0;
		for (std::vector<bool>::iterator it = byte.begin(); it != byte.end(); it++) {
			if (*it == true) {
				sum = sum + pow(2, counter);
			}
			counter++;
		}
		std::cout << sum << " ";
	}
	static void printBytes(std::vector<bool> bits) {
		while (bits.size() % 8 != 0) {
			bits.push_back(0);
		}
		for (size_t i = 0; i < bits.size() / 8; i++)
		{
			std::vector<bool> byte;
			for (size_t k = 0; k < 8; k++)
			{
				byte.push_back(bits[8 * i + k]);
			}
			printByte(byte);
		}
	}
	static std::vector<std::string> split(std::string command)
	{
		std::vector<std::string> parts;
		std::string buffer;
		for (std::string::iterator it = command.begin(); it != command.end(); it++) {
			if (*it != ' ') {
				buffer += *it;
			}
			else {
				parts.push_back(buffer);
				buffer = "";
			}
		}
		parts.push_back(buffer);
		return parts;
	}
	static void printMenu() {	// print menu
		std::cout << std::setw(84) << "Huffman's Algorithm for compression and decompression!" << std::endl;
		std::cout << std::endl;
		std::cout << std::setw(60) << "Information" << std::endl;
		std::cout << std::endl;
		std::cout << "In order to use the application properly, first you need to choose the file to load from and a file to output the result to. The order doesn't matter. Choosing the file to load from is like \"-i filename.txt\". Choosing the file to output to is like \"-o filename.txt\". Note that you MUST write the file extensions yourself! " << std::endl;
		std::cout << "When you load the input and output files, there are two ways of compressing. " << std::endl;
		std::cout << "1. The Static Way - it needs to know the information before hand." << std::endl;
		std::cout << "2. The Adaptive Way - it doesn't need to know any information." << std::endl;
		std::cout << "For the Static way there are two commands" << std::endl;
		std::cout << "\"-c\" -- to compress the input file and write the data in the output file. " << std::endl;
		std::cout << "\"-d\" -- to decompress the input file and write the data in the output file. For the Adaptive way there are also two commands." << std::endl;
		std::cout << "\"-cAdaptive\" for compression and \"-dAdaptive\" for decompression. " << std::endl;
		std::cout << "Note that with every successful operation, the input file and output file are resetted." << std::endl;
		std::cout << std::endl;
	}
	static std::string getAscii(const char& symbol) {
		int symbolInt = (int)symbol;
		std::string byte;
		while (symbolInt > 0) {
			byte.push_back((char)(symbolInt % 2 + '0'));
			symbolInt /= 2;
		}
		while (byte.length() < 8) {
			byte.push_back('0');
		}
		std::string reversedByte;
		for (int i = byte.length() - 1; i >= 0; i--) {
			reversedByte.push_back(byte[i]);
		}
		return reversedByte;
	}
	static char asciiToChar(std::string eightBits) {
		int charNumber = 0;
		int degrees = 0;
		for (int i = eightBits.length() - 1; i >= 0; i--)
		{
			if (eightBits[i] == '1') {
				charNumber = charNumber + pow(2, degrees);
			}
			degrees++;
		}
		return (char)charNumber;
	}
};
