#include "LexiBWT.h"
#include <iostream>

using namespace std;

LexiBWT::LexiBWT(int length) {
  this->length = length;
  this->ranks = (int *) calloc(this->length, sizeof(int));
  this->positions = (int *) calloc(this->length, sizeof(int));
  this->hvec.assign(256, std::vector<int>(0));
}

LexiBWT::~LexiBWT() {
}

int LexiBWT::transform(unsigned char * source, int * target) {
  this->source = source;
  this->target = target;

  sort();

  return this->ranks[0];
}

void LexiBWT::sort() {
  // Init vectors
  bzero(this->ranks, sizeof(int) * this->length);

  for (int i = 0; i < this->length; i++) {
    this->positions[i] = i;
  }

  int it = this->length - 1;
  while (it >= 0) {
    for (auto &h : this->hvec) {
      h.clear();
    }

    for (int i = 0; i < this->length; i++) {
      unsigned char c = this->source[get_char_idx(this->positions[i] + it)];
      this->hvec[c].push_back(this->positions[i]);
    }

    {
      int j = 0;
      for (int i = 0; i < 256; i++) {
        for (int v : this->hvec[i]) {
          this->positions[j] = v;
          j++;
        }
      }
    }

    it--;
  }

  for (int i = 0; i < this->length; i++) {
    this->ranks[this->positions[i]] = i;
  }

  this->target[this->ranks[0]] = this->source[this->length-1];
  for (int i = 1; i < this->length; i++) {
    this->target[this->ranks[i]] = this->source[i-1];
  }
}

int LexiBWT::get_char_idx(int idx) {
  if (idx >= this->length) {
    return (idx - this->length);
  } else {
    return idx;
  }
}
