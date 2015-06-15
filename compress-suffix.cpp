#include <iostream>
#include <cstring>
#include <cstdio>
#include "SuffixBWT.h"
#include "MoveToFront.h"
#include "Huffman.h"

using namespace std;

#define DEFAULT_CHUNK_SIZE 256

int main(int argc, char ** argv) {
  char * input_filename;
  char * output_filename;
  int max_chunk_size = DEFAULT_CHUNK_SIZE;

  clock_t cbeg, cend;
  long int cmtf = 0, chuff = 0, cbwt = 0;
  

  if (argc >= 3) {
    input_filename  = argv[1];
    output_filename = argv[2];
    if (argc >= 4) {
      max_chunk_size = atoi(argv[3]);
    }
  } else {
    cout << "Zle argumenty." << endl;
    cout << "./compress-suffix plik_wejsciowy plik_wyjsciowy rozmiar_okna=256" << endl;
    exit(EXIT_FAILURE);
  }

  // open source file
  FILE * input_file = fopen(input_filename, "rb");
  if (input_file == NULL) {
    cout << "Plik zrodlowy nie istnieje" << endl;
  }

  // calculate source file size
  fseek(input_file, 0, SEEK_END);
  int source_len = ftell(input_file);
  rewind(input_file);

#ifdef DEBUG
  cout << "Sourcelen: " << source_len << endl;
#endif

  unsigned char source[max_chunk_size + 1] = { 0 };
  int chunks, last_chunk_size;
  if (source_len % max_chunk_size == 0) {
    chunks = source_len / max_chunk_size;
    last_chunk_size = max_chunk_size;
  } else {
    chunks = (source_len / max_chunk_size) + 1;
    last_chunk_size = source_len % max_chunk_size;
  }

  MoveToFront * mtf = new MoveToFront();
  int mtf_tbl[max_chunk_size + 5];
  int target[max_chunk_size + 1];
  SuffixBWT * bwt = new SuffixBWT(max_chunk_size + 1, source, target);
  FILE * htarget = fopen(output_filename, "wb");
  Huffman * huffman1 = new Huffman(1, 0, 256);
  huffman1->Out = htarget;
  huffman1->compress_init(source_len + 5 * chunks);
  for (int i = 0; i < chunks; i++) {
    cout << "Compressing " << float(i) / (chunks - 1) * 100 << "%\n";
    int current_chunk_size;
    if (i == chunks-1) {
      current_chunk_size = last_chunk_size;
    } else {
      current_chunk_size = max_chunk_size;
    }

#ifdef DEBUG
    cout << "Chunk size " << i << ": " << current_chunk_size << endl;
#endif

    fread(source, current_chunk_size, 1, input_file);
    source[current_chunk_size] = 0;

#ifdef DEBUG
    cout << "Source: ";
    for (int j = 0; j < current_chunk_size + 1; j++) {
      cout << " " << +source[j];
    }
    cout << endl;
#endif

    // run bwt
    cbeg = clock();
    int orig_idx = bwt->transform(current_chunk_size + 1);
    cend = clock();
    cbwt += (cend - cbeg);

#ifdef DEBUG
    cout << "BWT: ";
    for (int j = 0; j < current_chunk_size + 1; j++) {
      cout << " " << target[j];
    }
    cout << "\nBWTsuccessful" << endl;
    cout << "Index of orig string: " << orig_idx << endl;
#endif

    // MoveToFront
    mtf->target = mtf_tbl;
    cbeg = clock();
    mtf->run(orig_idx);
    mtf->run(target, current_chunk_size + 1);
    cend = clock();
    cmtf += (cend - cbeg);

#ifdef DEBUG
    cout << "MTF:";
    for (int i = 0; i < current_chunk_size + 4 + 1; i++) {
      cout << " " << mtf_tbl[i];
    }
    cout << endl;
#endif

    // Huffman
    huffman1->data_in = mtf_tbl;
    cbeg = clock();
    huffman1->compress(current_chunk_size + 5);
    cend = clock();
    chuff += (cend - cbeg);
  }
  huffman1->compress_finish();
  fclose(htarget);
  fclose(input_file);

  delete bwt;
  delete mtf;
  delete huffman1;

  printf("BWT: %ld | MTF: %ld | HUFF: %ld\n", cbwt, cmtf, chuff);

  return 0;
}

