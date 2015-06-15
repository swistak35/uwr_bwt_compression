#ifndef MOVE_TO_FRONT_H
#define MOVE_TO_FRONT_H

#define MOVE_TO_FRONT_VERBOSE 0

#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <list>

class MoveToFront {
  public:
    MoveToFront();
    ~MoveToFront();
    void run(int * source, int count);
    void run(int source);
    void reset();
    int * target;
  private:
    int get_char(int c);
    int batchSize;
    std::list<int> table;
};

#endif
