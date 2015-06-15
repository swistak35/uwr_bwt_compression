#include <iostream>
#include <cstring>
#include <cstdio>
#include "DeBWT.h"
#include "DemoveToFront.h"
#include "Huffman.h"

using namespace std;

#define DEFAULT_CHUNK_SIZE 256
#define DEBUG 0

// Debug printing methods
void print_debug_source_len(int source_len);

int main(int argc, char ** argv) {
  char * input_filename;
  char * output_filename;
  int max_chunk_size = DEFAULT_CHUNK_SIZE;

  // Parse cmdline arguments
  if (argc >= 3) {
    input_filename  = argv[1];
    output_filename = argv[2];
    if (argc >= 4) {
      max_chunk_size = atoi(argv[3]);
    }
  } else {
    cout << "Zle argumenty." << endl;
    cout << "./decompress0 plik_wejsciowy plik_wyjsciowy rozmiar_okna=256" << endl;
    exit(EXIT_FAILURE);
  }

  // Open source file
  FILE * hsource = fopen(input_filename, "rb");

  // Initiate huffman class
  Huffman * huffman2 = new Huffman(0, 1, 256);
  huffman2->In = hsource;
  int mtf_tbl2[max_chunk_size + 4];

  // Extract original file length from compressed file
  int source_len2 = huffman2->decompress_init();

  // Calculate chunks size
  int chunks2, last_chunk_size2;
  if (source_len2 % (max_chunk_size + 4) == 0) {
    chunks2 = source_len2 / (max_chunk_size + 4);
    last_chunk_size2 = max_chunk_size;
  } else {
    chunks2 = (source_len2 / (max_chunk_size + 4)) + 1;
    last_chunk_size2 = (source_len2 % (max_chunk_size + 4)) - 4;
  }

  print_debug_source_len(source_len2);

  // Initialize Demove to front
  DemoveToFront * demtf = new DemoveToFront();
  int target2[max_chunk_size + 1 - 4] = { 0 };

  // Data structures for output file
  FILE * output_file = fopen(output_filename, "wb");
  unsigned char source2[max_chunk_size + 1 - 4] = { 0 };

  for (int i = 0; i < chunks2; i++) {
    cout << "Decompressing " << float(i) / chunks2 * 100 << "%\n";
    bzero(source2, max_chunk_size + 1 - 4);
    int current_chunk_size;
    if (i == chunks2-1) {
      current_chunk_size = last_chunk_size2;
    } else {
      current_chunk_size = max_chunk_size;
    }
    if (DEBUG) {
      cout << "Chunk size " << i << ": " << current_chunk_size << endl;
    }

    huffman2->data_out = mtf_tbl2;
    huffman2->decompress(current_chunk_size + 4);

    if (DEBUG) {
      cout << "MTF2:";
      for (int j = 0; j < current_chunk_size + 4; j++) {
        cout << " " << mtf_tbl2[j];
      }
      cout << endl;
    }


    demtf->source = mtf_tbl2;
    int orig_idx2;
    demtf->run(&orig_idx2);
    if (DEBUG) {
      cout << "Orig idx2: " << orig_idx2 << endl;
    }
    demtf->run(target2, current_chunk_size);
    if (DEBUG) {
      /* cout << "BWT2: " << target2 << endl; */
      cout << "BWT2: ";
      for (int j = 0; j < current_chunk_size; j++) {
        cout << " " << target2[j];
      }
      cout << "\nBWT2 successful" << endl;
    }

    // Decoding
    DeBWT * debwt = new DeBWT(current_chunk_size);
    debwt->transform(orig_idx2, target2, source2);
    /* if (DEBUG) { */
    /*   cout << "Source2: " << source2 << endl; */
    /* } */

    if (DEBUG) {
      cout << "Source:";
      for (int j = 0; j < current_chunk_size; j++) {
        cout << " " << +source2[j];
      }
      cout << endl;
    }
    fwrite(source2, current_chunk_size, 1, output_file);
  }
  fclose(hsource);
  fclose(output_file);

  return 0;
}

void print_debug_source_len(int source_len) {
  if (DEBUG) {
    cout << "Source len: " << source_len << endl;
  }
}
