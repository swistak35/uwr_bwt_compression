#include "SuffixBWT.h"
#include "LexiBWT.h"
#include <cstdio>
#include <strings.h>
#include <cstring>

using namespace std;

unsigned char txt[] = { 102, 100, 115, 97, 32, 102, 100, 115, 97, 10, 32, 102, 100, 115, 97, 32, 102, 100, 115, 97, 32, 102, 100, 115, 97, 32, 102, 100, 115, 10, 97, 102, 32, 100, 115, 97, 102, 100, 115, 97, 102, 100, 115, 10, 255, 0 };
/* unsigned char txt[] = { 102, 97, 102, 32, 97, 102, 255, 0 }; */
/* unsigned char txt[] = { 100, 100, 97, 100, 115, 97, 100, 115, 97, 255, 0 }; */
/* unsigned char txt[] = { 102, 97, 102, 32, 97, 102, 97, 102, 255, 0 }; */
/* unsigned char txt[] = { 100, 119, 97, 32, 115, 108, 111, 10, 255, 0 }; */
/* unsigned char txt[] = "ababbabbaabbabbz"; */
/* unsigned char txt[] = "cacaoz"; */

int size = sizeof(txt) - 1;

int main() {
  // pamietac ze z tymi size jest chyba cos nie tak!
  cout << "Size: " << size << endl;
  cout << "OrgText: " << txt << endl;

  int target1[size + 1] = { 0 };
  SuffixBWT * suffix_bwt = new SuffixBWT(size);
  int res1 = suffix_bwt->transform(txt, target1);
  cout << "Target1: ";
  for (int i = 0; i < size; i++) {
    cout << target1[i] << " ";
  }
  cout << endl;
  cout << "Target1: ";
  for (int i = 0; i < size; i++) {
    cout << (char) target1[i];
  }
  cout << endl;
  cout << "Res1: " << res1 << endl;

  int target2[size + 1] = { 0 };
  LexiBWT * lexi_bwt = new LexiBWT(size);
  int res2 = lexi_bwt->transform(txt, target2);
  cout << "Target2: ";
  for (int i = 0; i < size; i++) {
    cout << target2[i] << " ";
  }
  cout << endl;
  cout << "Target2: ";
  for (int i = 0; i < size; i++) {
    cout << (char) target2[i];
  }
  cout << endl;
  cout << "Res2: " << res2 << endl;

  return 0;
}
