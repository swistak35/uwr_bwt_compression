#ifndef UNI_BWT_H
#define UNI_BWT_H

#include <algorithm>
#include <vector>
#include <iostream>
#include <cstring>


class UniBWT {
  public:
    UniBWT(int length);
    ~UniBWT();
    void prepare(int length);
    int transform(unsigned char * source, int * target);
  private:
    int max_length;
    int hvecs;
    int * ranks;
    int * new_ranks;
    int * positions;
    std::vector<std::vector<int>> hvec;
    void sort();
    void first_iteration();
    void lex_sort(int offset, int * buckets);
    int get_char_idx(int idx);
    int length;
    unsigned char * source;
    int * target;
};

#endif
