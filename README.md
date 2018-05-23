# Huffman-Coding
A small C++ console application demonstrating the Huffman coding and Adaptive Huffman coding algorithms.

For our convenience and because of the learning purposes of the project, the encoded output is written down not to a binary file but to a text one.

Static Huffman coding

The algorithm serializes the encoded text as a binary/decimal sequence and the corresponding Huffman binary tree in order to be able to decode back the text in the future.

Adaptive Huffman coding
Unlike the static Huffman coding, the adaptive one does not require additional memory for saving the tree. The tree is constructed dynamically both during encoding and decoding. The most important part is that the encoding and decoding procedures finish the work having the same tree as a result, which follows the sibling property.
