#include "SuffixBWT.h"
#include <iostream>
#include <assert.h>
#include <cstdio>
#include <strings.h>

using namespace std;

SuffixBWT::SuffixBWT(int max_length, unsigned char * source, int * target) {
  this->max_length = max_length;

  this->source = source;
  this->target = target;

  this->tree = new SuffixTree(this->max_length, this->source);
  this->next_nodes_stack = (pair<BranchNode*,int> *) calloc(sizeof(pair<BranchNode*,int>), 2 * this->max_length + 4);
}

SuffixBWT::~SuffixBWT() {
  free(this->next_nodes_stack);
  delete this->tree;
}

int SuffixBWT::transform(int length) {
  this->length = length;
  this->tree->initialize(this->length);

  for (int i = 0; i < this->length; i++) {
    this->tree->insert_next();
  }

  int original_rank = set_ranks_root();

  return original_rank;
}

int SuffixBWT::set_ranks_root() {
  pair<BranchNode*,int> * next_nodes_list = next_nodes_stack;
  int current_position = 0;
  next_nodes_list->first = this->tree->root_node;
  next_nodes_list->second = 0;
  next_nodes_list++;

  BranchNode * node;
  int depth;
  int original_rank = -1;
  while (next_nodes_list > next_nodes_stack) {
    next_nodes_list--;
    node = next_nodes_list->first;
    depth = next_nodes_list->second;

    if (node->edges->empty()) {
      int suffix_id = this->length - depth;
      assert(suffix_id >= 0);
      assert(suffix_id < this->length);
      assert(current_position < this->length);
      if (suffix_id == 0) {
        this->target[current_position] = 0;
        original_rank = current_position;
      } else {
        this->target[current_position] = this->source[suffix_id - 1];
      }
      current_position++;
    } else {
      Edge * edge;
      for (map<int,Edge*>::reverse_iterator it = node->edges->rbegin(); it != node->edges->rend(); it++) {
        edge = (*it).second;
        next_nodes_list->first = edge->target;
        if (edge->endingChar == 1000000000) {
          next_nodes_list->second = depth + this->length - edge->startingChar;
        } else {
          next_nodes_list->second = depth + edge->endingChar - edge->startingChar + 1;
        }
        next_nodes_list++;
      }
    }
  }

  return original_rank;
}
