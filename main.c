#include "input.h"
#include "utils.h"

int main() {
  initialize();
  all_trains(stdin, stdout);
  find_nearest(stdin, stdout);
  free_all();
  return 0;
}
