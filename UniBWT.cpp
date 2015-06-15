#include "UniBWT.h"
#include <iostream>

using namespace std;

UniBWT::UniBWT(int max_length) {
  this->max_length = max_length;

  this->ranks = (int *) calloc(this->max_length, sizeof(int));
  if (this->ranks == NULL) {
    throw "Failed to allocate memory for UniBWT.ranks\n"; 
  }

  this->new_ranks = (int *) calloc(this->max_length, sizeof(int));
  if (this->new_ranks == NULL) {
    throw "Failed to allocate memory for UniBWT.new_ranks\n"; 
  }

  this->positions = (int *) calloc(this->max_length, sizeof(int));
  if (this->positions == NULL) {
    throw "Failed to allocate memory for UniBWT.positions\n"; 
  }

  if (this->max_length < 256) {
    this->hvecs = 256;
  } else {
    this->hvecs = this->max_length;
  }
  this->hvec.assign(this->hvecs, std::vector<int>(0));
}

UniBWT::~UniBWT() {
  free(this->ranks);
  free(this->new_ranks);
  free(this->positions);
}

void UniBWT::prepare(int length) {
  this->length = length;
}

int UniBWT::transform(unsigned char * source, int * target) {
  this->source = source;
  this->target = target;

  sort();

  return this->ranks[0];
}

void UniBWT::sort() {
  first_iteration();

  int buckets = 256;
  int offset = 1;
  while (offset <= this->length / 2) {
    lex_sort(offset, &buckets);
    offset <<= 1;
  }

  if (offset < this->length) {
    offset = this->length - offset;
    // possible optimization:
    //  last iteration doesn't have to do all this work
    lex_sort(offset, &buckets);
  }

  // make ranks unique
  for (int i = 0; i < this->length; i++) {
    this->ranks[this->positions[i]] = i;
  }

  this->target[this->ranks[0]] = this->source[this->length-1];
  for (int i = 1; i < this->length; i++) {
    this->target[this->ranks[i]] = this->source[i-1];
  }
}

void UniBWT::first_iteration() {
  for (int i = 0; i < 256; i++) {
    hvec[i].clear();
  }

  for (int i = 0; i < this->length; i++) {
    unsigned char c = this->source[get_char_idx(i - 1)];
    this->hvec[c].push_back(i);
  }

  int j = 0;
  for (int i = 0; i < 256; i++) {
    for (int v : this->hvec[i]) {
      this->positions[j] = v;
      this->ranks[v] = i;
      j++;
    }
  }
}

void UniBWT::lex_sort(int offset, int * buckets) {
  for (int i = 0; i < *buckets; i++) {
    hvec[i].clear();
  }

  for (int i = 0; i < this->length; i++) {
    int rank = this->ranks[get_char_idx(this->positions[i] - offset)];
    this->hvec[rank].push_back(this->positions[i]);
  }

  int j = 0;
  int current_rank = -1;
  int prev_rank_left = -1;
  int prev_rank_right = -1;
  for (int i = 0; i < *buckets; i++) {
    for (int v : this->hvec[i]) {
      this->positions[j] = v;
      int rank_right = this->ranks[v];
      int rank_left = this->ranks[get_char_idx(v - offset)];
      if (rank_left != prev_rank_left || rank_right != prev_rank_right) {
        current_rank++;
        prev_rank_left = rank_left;
        prev_rank_right = rank_right;
      }
      this->new_ranks[v] = current_rank;
      j++;
    }
  }
  *buckets = current_rank + 1;

  memcpy(this->ranks, this->new_ranks, sizeof(int) * this->length);
}

int UniBWT::get_char_idx(int idx) {
  if (idx >= this->length) {
    return (idx - this->length);
  } else if (idx < 0) {
    return (idx + this->length);
  } else {
    return idx;
  }
}
