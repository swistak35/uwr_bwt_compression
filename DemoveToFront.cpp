#include "DemoveToFront.h"

using namespace std;

DemoveToFront::DemoveToFront() {
  this->reset();
}

DemoveToFront::~DemoveToFront() {
}

void DemoveToFront::reset() {
  this->table.clear();
  for (int i = 0; i < 256; i++) {
    this->table.push_back(i);
  }
}

void DemoveToFront::run(int * i) {
  *i = 0;
  int ti;
  unsigned char c;
  for (int j = 0; j < 4; j++) {
    c = (unsigned char) get_char(*this->source);
    this->source++;
    ti = c << (8 * j);
    *i = *i | ti;
  }
}

void DemoveToFront::run(int * target, int count) {
  for (int i = 0; i < count; i++) {
    target[i] = get_char(*this->source);
    this->source++;
  }
}

int DemoveToFront::get_char(int pos) {
  list<int>::iterator it = this->table.begin();

  for (int i = 0; i < pos; i++) {
    it++;
  }

  int c = *it;
  this->table.erase(it);
  this->table.push_front(c);

  return c;
}
