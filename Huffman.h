#ifndef HUFFMAN_H

#define HUFFMAN_H

#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>

typedef struct {
    unsigned int up,      // next node up the tree
        down,         // pair of down nodes
        symbol,       // node symbol value
        weight;       // node weight
} HTable;

typedef struct {
    unsigned int esc,     // the current tree height
        root,         // the root of the tree
        size,         // the alphabet size
        * map;         // mapping for symbols to nodes
    HTable table[1];  // the coding table starts here
} HCoder;

class Huffman {
  public:
    // basic stuff
    Huffman(int source_type, int target_type, int alphabet_size);
    ~Huffman();

    // from original algorithm
    void huff_init(unsigned int size, unsigned int root);
    unsigned int huff_split(HCoder * huff, unsigned int symbol);
    void huff_increment(HCoder * huff, unsigned int node);
    void huff_scale(unsigned int bits);
    void huff_sendid(HCoder *huff, unsigned symbol);
    void huff_encode(unsigned int symbol);
    unsigned huff_readid(HCoder *huff);
    unsigned huff_decode();

    void display_map(int x, HCoder * hcoder);
    void display_map(int x);
    void display_hcoder(HCoder * hcoder);
    void arc_put1(unsigned bit);
    unsigned int arc_get1();
    HCoder * hcoder;
    unsigned char ArcBit;
    int ArcChar;
    int alphabet_size;
    int get_next_char();
    void put_next_char(int c);
    void compress_init(int size);
    void compress(int max);
    void compress_finish();
    int decompress_init();
    int decompress(int max);
    int length;
    FILE * Out;
    FILE * In;
    int * data_out;
    int * data_in;
    int source_type;
    int target_type;
};
#endif
