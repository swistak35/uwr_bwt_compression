#ifndef LEXI_BWT_H
#define LEXI_BWT_H

#include <algorithm>
#include <vector>
#include <iostream>
#include <strings.h>

class LexiBWT {
  public:
    LexiBWT(int length);
    ~LexiBWT();
    int transform(unsigned char * source, int * target);
  private:
    int * ranks;
    int * positions;
    std::vector<std::vector<int>> hvec;
    void sort();
    int get_char_idx(int idx);
    int length;
    unsigned char * source;
    int * target;
};

#endif
