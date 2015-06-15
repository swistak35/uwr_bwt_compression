#include "Huffman.h"

using namespace std;

#define __cdecl

void arc_put1(unsigned bit);
unsigned arc_get1();

Huffman::Huffman(int source_type, int target_type, int alphabet_size) {
  this->source_type = source_type;
  this->target_type = target_type;
  this->alphabet_size = alphabet_size;
  ArcBit = 0;
  ArcChar = 0;
}

Huffman::~Huffman() {
  free(this->hcoder->map);
  free(this->hcoder);
}

void Huffman::huff_init(unsigned int size, unsigned int root) {
  HCoder * huff;

  //  default: all alphabet symbols are used
  if( !root || root > size ) {
    root = size;
  }

  //  create the initial escape node
  //  at the tree root

  if( root <<= 1 ) {
    root--;
  }

  huff = (HCoder *) malloc(root * sizeof(HTable) + sizeof(HCoder));
  memset(huff->table + 1, 0, root * sizeof(HTable));
  memset(huff, 0, sizeof(HCoder));

  huff->size = size;
  if( huff->size ) {
    huff->map = (unsigned int *) calloc(size, sizeof(unsigned int));
  }

  huff->esc = huff->root = root;
  this->hcoder = huff;
}

// split escape node to incorporate new symbol

unsigned int Huffman::huff_split(HCoder *huff, unsigned symbol) {
  unsigned pair, node;

  //  is the tree already full???

  pair = huff->esc;
  if( pair ) {
    huff->esc--;
  } else {
    return 0;
  }

  //  if this is the last symbol, it moves into
  //  the escape node's old position, and
  //  huff->esc is set to zero.

  //  otherwise, the escape node is promoted to
  //  parent a new escape node and the new symbol.

  node = huff->esc;
  if( node ) {
    huff->table[pair].down = node;
    huff->table[pair].weight = 1;
    huff->table[node].up = pair;
    huff->esc--;
  } else {
    pair = 0;
    node = 1;
  }

  //  initialize the new symbol node

  huff->table[node].symbol = symbol;
  huff->table[node].weight = 0;
  huff->table[node].down = 0;
  /* printf("Przypisywanie %u do %u\n", node, symbol); */
  huff->map[symbol] = node;

  //  initialize a new escape node.

  huff->table[huff->esc].weight = 0;
  huff->table[huff->esc].down = 0;
  huff->table[huff->esc].up = pair;
  return node;
}

//  swap leaf to group leader position
//  return symbol's new node

unsigned int huff_leader(HCoder * huff, unsigned int node) {
  unsigned int weight = huff->table[node].weight;
  unsigned int leader = node, prev, symbol;

  while ( weight == huff->table[leader + 1].weight ) {
    leader++;
  }

  if ( leader == node ) {
    return node;
  }

  // swap the leaf nodes

  symbol = huff->table[node].symbol;
  prev = huff->table[leader].symbol;

  huff->table[leader].symbol = symbol;
  huff->table[node].symbol = prev;
  /* printf("Leader Przypisywanie %u do %u\n", leader, symbol); */
  huff->map[symbol] = leader;
  /* printf("Leader Przypisywanie %u do %u\n", node, prev); */
  huff->map[prev] = node;
  return leader;
}

//  slide internal node up over all leaves of equal weight;
//  or exchange leaf with next smaller weight internal node

//  return node's new position

unsigned int huff_slide (HCoder * huff, unsigned int node) {
  unsigned next = node;
  HTable swap[1];

  *swap = huff->table[next++];

  // if we're sliding an internal node, find the
  // highest possible leaf to exchange with

  if ( swap->weight & 1 ) {
    while( swap->weight > huff->table[next + 1].weight ) {
      next++;
    }
  }

  //  swap the two nodes

  huff->table[node] = huff->table[next];
  huff->table[next] = *swap;

  huff->table[next].up = huff->table[node].up;
  huff->table[node].up = swap->up;

  //  repair the symbol map and tree structure

  if( swap->weight & 1 ) {
    huff->table[swap->down].up = next;
    huff->table[swap->down - 1].up = next;
    /* printf("slide1 Przypisywanie %u do %u\n", node, huff->table[node].symbol); */
    huff->map[huff->table[node].symbol] = node;
  } else {
    huff->table[huff->table[node].down - 1].up = node;
    huff->table[huff->table[node].down].up = node;
    /* printf("slide2 Przypisywanie %u do %u\n", next, swap->symbol); */
    huff->map[swap->symbol] = next;
  }

  return next;
}

//  increment symbol weight and re balance the tree.

void Huffman::huff_increment(HCoder * huff, unsigned int node) {
  unsigned up;

  //  obviate swapping a parent with its child:
  //    increment the leaf and proceed
  //    directly to its parent.

  //  otherwise, promote leaf to group leader position in the tree

  if( huff->table[node].up == node + 1 ) {
    huff->table[node].weight += 2, node++;
  } else {
    node = huff_leader(huff, node);
  }

  //  increase the weight of each node and slide
  //  over any smaller weights ahead of it
  //  until reaching the root

  //  internal nodes work upwards from
  //  their initial positions; while
  //  symbol nodes slide over first,
  //  then work up from their final
  //  positions.

  while( huff->table[node].weight += 2, up = huff->table[node].up ) {
    while( huff->table[node].weight > huff->table[node + 1].weight ) {
      node = huff_slide (huff, node);
    }

    if( huff->table[node].weight & 1 ) {
      node = up;
    } else {
      node = huff->table[node].up;
    }
  }
}

//  scale all weights and rebalance the tree

//  zero weight nodes are removed from the tree
//  by sliding them out the left of the rank list

void Huffman::huff_scale(unsigned int bits) {
  HCoder * huff = this->hcoder;
  unsigned int node = huff->esc, weight, prev;

  //  work up the tree from the escape node
  //  scaling weights by the value of bits

  while( ++node <= huff->root ) {
    //  recompute the weight of internal nodes;
    //  slide down and out any unused ones

    if( huff->table[node].weight & 1 ) {
      weight = huff->table[huff->table[node].down].weight & ~1;
      if( weight ) {
        weight += huff->table[huff->table[node].down - 1].weight | 1;
      }

    //  remove zero weight leaves by incrementing HuffEsc
    //  and removing them from the symbol map.  take care

    } else if( !(weight = huff->table[node].weight >> bits & ~1) )
      if( huff->map[huff->table[node].symbol] = 0, huff->esc++ )
        huff->esc++;

    // slide the scaled node back down over any
    // previous nodes with larger weights

    huff->table[node].weight = weight;
    prev = node;

    while( weight < huff->table[--prev].weight )
        huff_slide (huff, prev);
  }

  // prepare a new escape node

  huff->table[huff->esc].down = 0;
}

//  send the bits for an escaped symbol

void Huffman::huff_sendid (HCoder *huff, unsigned symbol) {
  unsigned empty = 0, max;

    //  count the number of empty symbols
    //  before the symbol in the table

    while( symbol-- )
      if( !huff->map[symbol] )
        empty++;

    //  send LSB of this count first, using
    //  as many bits as are required for
    //  the maximum possible count

    max = huff->size - (huff->root - huff->esc) / 2 - 1;
    if( max ) {
      do arc_put1 (empty & 1), empty >>= 1;
      while( max >>= 1 );
    }
}

//  encode the next symbol

void Huffman::huff_encode(unsigned int symbol) {
  HCoder * huff = this->hcoder;
  unsigned emit = 1, bit;
  unsigned up, idx, node;

    /* cout << "Dupa0.1: " << symbol <<  endl; */
    if( symbol < huff->size )
        node = huff->map[symbol];
    else
        return;

    //  for a new symbol, direct the receiver to the escape node
    //  but refuse input if table is already full.

    /* cout << "Dupa0.2: " << node <<  endl; */
    if( !(idx = node) )
      if( !(idx = huff->esc) )
        return;

    /* cout << "Dupa0.5: " << idx << endl; */

    //  accumulate the code bits by
    //  working up the tree from
    //  the node to the root

    up = huff->table[idx].up;
    /* cout << "Dupa0.6" << endl; */
    while( up ) {
      emit <<= 1, emit |= idx & 1, idx = up;
      up = huff->table[idx].up;
    }

    //  send the code, root selector bit first

    /* cout << "Dupa1" << endl; */
    while( bit = emit & 1, emit >>= 1 )
        arc_put1 (bit);

    /* cout << "Dupa2" << endl; */
    //  send identification and incorporate
    //  new symbols into the tree

    /* display_map(1, huff); */
    //
    if( !node )
        huff_sendid(huff, symbol), node = huff_split(huff, symbol);

    //  adjust and re-balance the tree

    /* display_map(2, huff); */
    huff_increment (huff, node);
}

//  read the identification bits
//  for an escaped symbol

unsigned Huffman::huff_readid (HCoder *huff)
{
unsigned empty = 0, bit = 1, max, symbol;

    //  receive the symbol, LSB first, reading
    //  only the number of bits necessary to
    //  transmit the maximum possible symbol value

    max = huff->size - (huff->root - huff->esc) / 2 - 1;
    if( max ) {
      do empty |= arc_get1 () ? bit : 0, bit <<= 1;
      while( max >>= 1 );
    }

    //  the count is of unmapped symbols
    //  in the table before the new one

    for( symbol = 0; symbol < huff->size; symbol++ )
      if( !huff->map[symbol] )
        if( !empty-- )
            return symbol;

    //  oops!  our count is too big, either due
    //  to a bit error, or a short node count
    //  given to huff_init.

    return 0;
}

//  decode the next symbol

unsigned Huffman::huff_decode ()
{
  HCoder * huff = this->hcoder;
  unsigned node = huff->root;
  unsigned symbol, down;

    //  work down the tree from the root
    //  until reaching either a leaf
    //  or the escape node.  A one
    //  bit means go left, a zero
    //  means go right.

    down = huff->table[node].down;
    while( down ) {
      if( arc_get1 () ) {
        node = down - 1;  // the left child preceeds the right child
      } else {
        node = down;
      }
      down = huff->table[node].down;
    }

    //  sent to the escape node???
    //  refuse to add to a full tree

    if( node == huff->esc )
      if( huff->esc )
        symbol = huff_readid (huff), node = huff_split (huff, symbol);
      else
        return 0;
    else
        symbol = huff->table[node].symbol;

    //  increment weights and rebalance
    //  the coding tree

    huff_increment (huff, node);
    return symbol;
}

void Huffman::arc_put1 (unsigned bit) {
  ArcChar <<= 1;

  if (bit) {
    ArcChar |= 1;
  }

  ArcBit++;
  if (ArcBit < 8) {
    return;
  }

  put_next_char(ArcChar);
  ArcChar = 0;
  ArcBit = 0;
}

unsigned int Huffman::arc_get1() {
  if (!ArcBit) {
    ArcChar = get_next_char();
    ArcBit = 8;
  }

  return ArcChar >> --ArcBit & 1;
}

void Huffman::compress_init(int size) {
  this->length = size;
  huff_init(this->alphabet_size, this->alphabet_size);
  /* display_hcoder(this->hcoder); */
  put_next_char(this->alphabet_size >> 8);
  put_next_char(this->alphabet_size);

  put_next_char(size >> 24);
  put_next_char(size >> 16);
  put_next_char(size >> 8);
  put_next_char(size);
  /* display_map(4); */
}

void Huffman::display_map(int x, HCoder * hcoder) {
  printf("%d map", x);
  for (unsigned int i=0; i<hcoder->size; i++) {
    printf(" %u", hcoder->map[i]);
  }
  printf("\n");
}

void Huffman::display_map(int x) {
  printf("%d map", x);
  for (unsigned int i=0; i<this->hcoder->size; i++) {
    printf(" %u", this->hcoder->map[i]);
  }
  printf("\n");
}

void Huffman::display_hcoder(HCoder * hcoder) {
  printf("-----------------------------\n");
  printf("esc: %u root %u size %u\n", hcoder->esc, hcoder->root, hcoder->size);
  for (int i = 0; i < 511; i++) {
    printf("(u %u d %u s %u w %u) \n",
      hcoder->table[0].up,
      hcoder->table[0].down,
      hcoder->table[0].symbol,
      hcoder->table[0].weight);
  }
  /* display_map(0, hcoder); */
}

void Huffman::compress(int max) {
  int i = 0;
  int symbol;
  unsigned mask = ~0;

  while ( this->length && i < max ) {
    /* display_map(9); */
    symbol = get_next_char();
    /* cout << "Encoding " << symbol << endl; */
    /* display_map(10); */
    huff_encode(symbol);
    /* display_map(11); */
    /* cout << "Encoded." << endl; */
    if ( this->length & mask ) {
      this->length--;
      i++;
      continue;
    } else {
      i++;
      huff_scale(1);
    }
  }
}

void Huffman::compress_finish() {
  while (ArcBit) { // flush last few bits
    arc_put1(0);
  }
}

int Huffman::decompress_init() {
  int size = this->alphabet_size;
  size = get_next_char() << 8;
  size |= get_next_char();

  huff_init(this->alphabet_size, this->alphabet_size);

  size = get_next_char() << 24;
  size |= get_next_char() << 16;
  size |= get_next_char() << 8;
  size |= get_next_char();
  this->length = size;
  return size;
}

int Huffman::decompress(int max) {
  int i = 0;
  int symbol;
  unsigned mask = ~0;
  while( this->length && i < max ) {
    if( symbol = huff_decode(), put_next_char(symbol), this->length-- & mask ) {
      i++;
      continue;
    } else {
      i++;
      huff_scale(1);
    }
  }
  return 0;
}

int Huffman::get_next_char() {
  if (this->source_type == 0) {
    return getc(In);
  } else if (this->source_type == 1) {
    unsigned char c = *this->data_in;
    this->data_in++;
    return c;
  } else {
    return 0;
  }
}

void Huffman::put_next_char(int c) {
  if (this->target_type == 0) {
    putc(c, Out);
  } else if (this->target_type == 1) {
    *this->data_out = (unsigned char) c;
    this->data_out++;
  }
}
