#ifndef DEBWT_H
#define DEBWT_H

#include <algorithm>
#include <vector>

class DeBWT {
  public:
    DeBWT(int length);
    ~DeBWT();
    void transform(int orig_idx, int * source, unsigned char * target);
  private:
    int length;
};

#endif
