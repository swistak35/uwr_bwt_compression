#ifndef STACK_TREE_MEMORY_H
#define STACK_TREE_MEMORY_H

#include <algorithm>
#include <cstdio>
#include <map>
#include "SuffixTreeStructures.h"

using namespace std;

class StackTreeMemory {
  public:
    StackTreeMemory(int max_length);
    ~StackTreeMemory();
    void reset();
    BranchNode * create_bnode();
    Edge * create_edge();
  private:
    int max_length;

    // BranchNodes
    int bnodes_count;
    int bnodes_counter;
    BranchNode * bnodes_stack;
    BranchNode * bnodes_stack_ptr;

    // Edges
    int edges_count;
    Edge * edges_stack;
    Edge * edges_stack_ptr;
};

#endif
