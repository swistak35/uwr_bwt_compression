#ifndef DEMOVE_TO_FRONT_H
#define DEMOVE_TO_FRONT_H

#define DEMOVE_TO_FRONT_VERBOSE 0

#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <list>

class DemoveToFront {
  public:
    DemoveToFront();
    ~DemoveToFront();
    void run(int * target, int count);
    void run(int * target);
    void reset();
    int * source;

  private:
    int get_char(int pos);
    std::list<int> table;
};

#endif
