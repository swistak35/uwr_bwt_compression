#include "StackTreeMemory.h"

StackTreeMemory::StackTreeMemory(int max_length) {
  this->max_length = max_length;

  this->bnodes_count = 2 * max_length + 4;
  this->bnodes_stack = (BranchNode *) calloc(this->bnodes_count, sizeof(BranchNode));
  if (this->bnodes_stack == NULL) {
    printf("Malloc failed for StackTreeMemory#bnodes_stack\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < this->bnodes_count; i++) {
    this->bnodes_stack[i].edges = new map<int,Edge*>();
  }

  this->edges_count = 2 * max_length + 4 + 257;
  this->edges_stack = (Edge *) calloc(this->edges_count, sizeof(Edge));
  if (this->edges_stack == NULL) {
    printf("Malloc failed for StackTreeMemory#edges_stack\n");
    exit(EXIT_FAILURE);
  }
}

StackTreeMemory::~StackTreeMemory() {
  for (int i = 0; i < this->bnodes_count; i++) {
    delete bnodes_stack[i].edges;
  }
  free(this->bnodes_stack);
  free(this->edges_stack);
}

BranchNode * StackTreeMemory::create_bnode() {
  BranchNode * ptr = bnodes_stack_ptr;
  ptr->edges->clear();
  ptr->longestProperSuffix = NULL;
  ptr->debugchar = bnodes_counter;

  bnodes_counter++;
  bnodes_stack_ptr++;
  return ptr;
}

Edge * StackTreeMemory::create_edge() {
  Edge * ptr = edges_stack_ptr;
  edges_stack_ptr++;
  return ptr;
}

void StackTreeMemory::reset() {
  this->bnodes_counter = 0;
  this->bnodes_stack_ptr = this->bnodes_stack;
  this->edges_stack_ptr = this->edges_stack;
}
