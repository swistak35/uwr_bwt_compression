#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <algorithm>
#include "SuffixTreeStructures.h"
#include "StackTreeMemory.h"

using namespace std;

class SuffixTree {
  public:
    SuffixTree(int max_length, unsigned char * source);
    ~SuffixTree();
    void initialize(int length);
    void insert_next();
    BranchNode * root_node;
  private:
    int max_length;
    void sort();
    int startingChar;
    int current_char;
    BranchNode * current_node;

    int get_digit(unsigned char * chr_ptr);
    int length;
    unsigned char * source;
    unsigned char * source_end;

    StackTreeMemory * memory;

    BranchNode * pin_node;
    BranchNode * create_branch_node();
    Edge * create_edge();
    Edge * find_edge_on_list(BranchNode * node, int c);
    void insert_edge_into_bnode(BranchNode * node, Edge * edge);
    void canonize(BranchNode * node, int endingChar);
    bool test_and_split(int endingChar, int current_char, BranchNode ** bnode);
    void update(int endingChar);

    void print_node(int depth, BranchNode * node);
    void print_tree(BranchNode * root_node);
    void print_tabs(int depth);
    void print_file();
};

#endif

