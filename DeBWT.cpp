#include "DeBWT.h"

using namespace std;

DeBWT::DeBWT(int length) {
  this->length = length;
}

DeBWT::~DeBWT() {
}

void DeBWT::transform(int orig_idx, int * source, unsigned char * target) {
  // pamietac w koncowej implementacji usunac + 1 i dodawanie bajtu zerowego!
  int * sorted = (int *) calloc(this->length + 1, sizeof(int));

  std::vector<std::vector<int>> hvec;
  hvec.assign(257, std::vector<int>(0));

  for (int i = 0; i < this->length; i++) {
    int c = source[i];
    hvec[c].push_back(i);
  }

  {
    int j = 0;
    for (int i = 0; i <= 256; i++) {
      for (int v : hvec[i]) {
        sorted[j] = v;
        j++;
      }
    }
  }

  {
    int i = orig_idx;

    // first iteration
    unsigned char c = (unsigned char) source[i];
    target[this->length - 1] = c;
    i = sorted[i];

    // rest of the iterations
    for (int j = 1; j < this->length; j++) {
      c = (unsigned char) source[i];
      target[j - 1] = c;
      i = sorted[i];
    }
  }
}
